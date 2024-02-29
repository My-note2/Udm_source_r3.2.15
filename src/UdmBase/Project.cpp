/*
Copyright (c) Vanderbilt University, 2000-2001
ALL RIGHTS RESERVED
Vanderbilt University disclaims all warranties with regard to this
software, including all implied warranties of merchantability
and fitness.  In no event shall Vanderbilt University be liable for
any special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether
in an action of contract, negligence or other tortious action,
arising out of or in connection with the use or performance of
this software.
*/
/*
03/26/06	-	endre
		-	make DeCompress() work with filenames longer than 50 characters

12/31/05	-	endre
		-	added CreateNewMeta() to UdmProject to create a new meta specifier UDM project

11/23/05	-	endre
		-	free string returned by _tempnam/tempnam

12/12/04	-	endre

		-	G++ 3 porting issues.

26/11/04	-	endre

		- fixed trivial  bug in line 663, 
		if (rev_i == datanetworks.end()) instead of
		if (rev_i == rev_datanetworks.end())
05/12/04	-	endre


		- fixed small bug in ~StaticUdmProject: cross_links StaticDataNetwork was not deleted
		and created problems when the process exited and the datanetworks were deleted from
		UdmStatic::id_map_t::~id_map_t()
  
03/03/04 - endre
		
		- added Close() to StaticUdmproject, which basically does what ~StaticUdmProject() did. 
		~StaticUdmProject now just invokes Close().

		- changed the StaticUdmProject constructor to use GetDataNetwork() call on StaticDataNetworkSpecifier, 
		to get a pointer to the real datanetwork.

*/
//tmpnam
#include <cstdio>

// memset
#include <cstring>

//UDM
#include <UdmBase.h>
#include <UmlExt.h>
#include <UdmStatic.h>

//zip 
#if defined(_M_AMD64)
#define ZLIB_WINAPI
#endif
#include <zlib.h>

#ifdef WIN32
#include <contrib\minizip\zip.h>
#include <contrib\minizip\unzip.h>
//mkdir, unlink
#include <direct.h>
//findfirstfile
#include <Windows.h>

#else	//WIN32

#include <minizip/zip.h>
#include <minizip/unzip.h>
#include <unistd.h>
#include <utime.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif	//WIN32

//UdmProject
#include <UdmProject.h>
#include <Uml.h>


//DTD generator
#include "DTDGen.h"

//ofstream
#include <fstream>

//UdmDom DomPath
#include <UdmDom.h>

void _udm_project_dummy_init(){};

#define WIN_TEMP_ENV "TEMP"
#define WIN_TMP_ENV "TMP"

#ifdef WIN32
#define PATHDELIM "\\"
#else	//WIN32
#define PATHDELIM "/"
#endif	//WIN32

/*

  CHANGELOG

	01/26/04 - Mainly porting issues to g++ 2.96, specially the compress/decompress stuff.
				void type had to be add to all the functions declared with no return type

	01/07/04 - Fixed a minor bug regarding UdmStaticProject reported by Feng

	12/12/03 =	Changed the cross class-diagram delimiter string rom "_" to "_cross_ph_"
*/

namespace UDM_NAMESPACE
{

	UDM_DLL UdmProject::UdmProject(bool static_pr) : cross_meta_dn(NULL), cross_meta(NULL), cross_links(NULL), zipfile(""), project_dn(NULL)
	{
		if (static_pr) return;//if it's a static datanetwork we don't do shit ;)
			
#ifdef WIN32
		const char * def_tmp_dir = "c:\\";
#else
		const char * def_tmp_dir = "/tmp/";
#endif
		
		const char * tmp_dir = NULL;
		tmp_dir = getenv(WIN_TEMP_ENV);
		if (!tmp_dir)
			tmp_dir = getenv(WIN_TMP_ENV);
		
		if (!tmp_dir) tmp_dir = def_tmp_dir;
		
		//create a temporary folder and set the path
		//char t_dir_name[L_tmpnam +1];
		char * t_dir_name;
#ifdef WIN32
		t_dir_name = _tempnam(tmp_dir, "UDM");
		_mkdir(t_dir_name);
#else	//WIN32
		t_dir_name = tempnam(tmp_dir, "UDM");
		if (mkdir(t_dir_name, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)) {
			throw udm_exception(string("Could not create temporary directory (")+string(t_dir_name)+string(")"));
		}
#endif	//WIN32
		

		//UdmDom::DomDataNetwork::DTDPath = t_dir_name;
			
		temp_path = t_dir_name;		
		free(t_dir_name);
	};

	void UdmProject::DeCompress()
	{
		//copy out all the files from the Zip file to the temp directory

		if (!zipfile.size())
			throw udm_exception("Internal UDM error: DeCompress() invoked but no project file is found!");


		if (!temp_path.size())
			throw udm_exception("Internal UDM error: DeCompress() invoked but no temporary directory is set!");

		unzFile zf = unzOpen(zipfile.c_str());
		if (!zf)
			throw udm_exception(string("File ") + zipfile + string("could not be opened. It must be corrupted."));


		int unz_res = unzGoToFirstFile(zf);

		while(unz_res == UNZ_OK)
		{
			long filename_length;
#ifdef WIN32
			filename_length = _MAX_PATH;
#else
			filename_length = pathconf(temp_path.c_str(), _PC_PATH_MAX);
			if (filename_length == -1) filename_length = 4096;
#endif
			filename_length -= temp_path.length() + strlen(PATHDELIM);
			char * filename = new char[filename_length];

			unz_file_info ufi;
			if (unzGetCurrentFileInfo(zf, &ufi, filename, filename_length, NULL, 0, NULL, 0) != UNZ_OK)
				throw udm_exception("Unknown UNZ error occurred!");

			string tempfilename = temp_path + PATHDELIM + filename;
			delete [] filename;

			if (unzOpenCurrentFile(zf) != UNZ_OK)
				throw udm_exception("Unknown UNZ error occurred!");
			
			char * buff = new char[ufi.uncompressed_size];
			if ((unsigned long)unzReadCurrentFile(zf, buff, ufi.uncompressed_size) != ufi.uncompressed_size)
				throw udm_exception("Unknown UNZ error occurred!");

			if (unzCloseCurrentFile(zf) != UNZ_OK)
				throw udm_exception("Unknown UNZ error occurred!");
			

			FILE * file = fopen(tempfilename.c_str(), "wb");
			if (!file)
				throw udm_exception(string("Temporary file could not be opened! (") + tempfilename + string(")"));

			fwrite(buff, sizeof(char), ufi.uncompressed_size, file);
			delete [] buff;
			fclose(file);

			
			unz_res = unzGoToNextFile(zf);
		}  

		if (unzClose(zf) != UNZ_OK)
			throw udm_exception(string("Internal UDM error: Could not close project file: ") + zipfile);

		
		
	};



	void UdmProject::Compress()
	{
		const int writebuffersize=65536;
		int opt_compress_level=Z_DEFAULT_COMPRESSION;
#ifdef WIN32
		HANDLE hFind;
		FILETIME ftLocal;
		BOOL findres;
		WIN32_FIND_DATA  ff32;
#else 	//WIN32
		DIR *dir;
		struct dirent *dent;
		struct stat st;
		time_t tm_t=0;
		struct tm* filedate;
#endif	//WIN32

		void *buf=(void *)malloc(writebuffersize);

		int err, readsize;
		FILE *fin;

		zipFile zf;
		if (sem != Udm::CHANGES_LOST_DEFAULT)
		{
			if (!(zf = zipOpen(zipfile.c_str(),0))) {
				throw(udm_exception(string("File ") + zipfile + string("could not be opened for writing.")));
			}
		}

#ifdef WIN32
		string project_files = temp_path + "\\*.*";
		findres = ( (hFind = FindFirstFile(project_files.c_str(),&ff32)) != INVALID_HANDLE_VALUE);
		while (findres) 
		{
			if (!(ff32.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
			{
				string findfn=temp_path + PATHDELIM + string(ff32.cFileName);
#else	//WIN32
		if (!(dir=opendir(temp_path.c_str()))) {
			throw(udm_exception(string("Could not open temporary directory (") + temp_path + string(")")));
		}
		while ( (dent=readdir(dir)) ) {
			string findfn=temp_path + "/" + string(dent->d_name);
			if ( (stat(findfn.c_str(),&st)==0) && (!S_ISDIR(st.st_mode)) ) {
						
#endif	//WIN32

				if (sem != Udm::CHANGES_LOST_DEFAULT)
				{
					zip_fileinfo zi;
					memset(&zi, 0, sizeof(zip_fileinfo));

#ifdef WIN32
					FileTimeToLocalFileTime(&(ff32.ftLastWriteTime),&ftLocal);
					FileTimeToDosDateTime(&ftLocal,((LPWORD)&zi.dosDate)+1,(LPWORD)&zi.dosDate);
#else	//WIN32				
					filedate = localtime(&tm_t);
					zi.tmz_date.tm_sec  = filedate->tm_sec;
					zi.tmz_date.tm_min  = filedate->tm_min;
					zi.tmz_date.tm_hour = filedate->tm_hour;
					zi.tmz_date.tm_mday = filedate->tm_mday;
					zi.tmz_date.tm_mon  = filedate->tm_mon ;
					zi.tmz_date.tm_year = filedate->tm_year;
#endif	//WIN32

#ifdef	WIN32
					if ( (err = zipOpenNewFileInZip(zf, ff32.cFileName, &zi,
									NULL,0,NULL,0,NULL, 
									(opt_compress_level != 0) ? Z_DEFLATED : 0,
									opt_compress_level)) != ZIP_OK) {
						throw(udm_exception(string("Could not create file ") + ff32.cFileName + string(" inside zip file")));
					}
#else	//WIN32
					if ( (err = zipOpenNewFileInZip(zf, dent->d_name, &zi,
									NULL,0,NULL,0,NULL, 
									(opt_compress_level != 0) ? Z_DEFLATED : 0,
									opt_compress_level)) != ZIP_OK) {
						throw(udm_exception(string("Could not create file ") + dent->d_name + string(" inside zip file")));
					}
#endif	//WIN32

					if ( !(fin = fopen(findfn.c_str(), "rb"))) {
						throw(udm_exception(string("File ") + findfn + string(" could not be opened for reading")));
					}

					do {
						readsize=fread(buf, 1, writebuffersize, fin);
							if (readsize) {
								if ( (err = zipWriteInFileInZip( zf, buf, readsize)) < 0) {
									throw(udm_exception(string("Could not write into zip file (") + findfn + string(")")));
								}
							}
					} while (readsize);
					fclose(fin);
					if ( (err = zipCloseFileInZip(zf)) != ZIP_OK ) {
						throw(udm_exception(string("Could not close file ") + findfn + string("in zip archive")));
					}
				}

#ifdef WIN32
				_unlink(findfn.c_str());
#else	//WIN32
				unlink(findfn.c_str());
#endif	//WIN32
			}

#ifdef WIN32
			findres = FindNextFile(hFind, &ff32); 
		}
		FindClose(hFind);
#else	//WIN32
		}
		closedir(dir);
#endif	//WIN32
		
		if (sem != Udm::CHANGES_LOST_DEFAULT)
		{
			if ( (err = zipClose(zf, NULL)) != ZIP_OK) {
				throw(udm_exception(string("Could not close zip file ") + zipfile));
			}
		}
		free(buf);
	};
	

	void UDM_DLL UdmProject::OpenExisting(const string & project_file, enum BackendSemantics _sem)
	{



		if (Project)
			throw udm_exception("Project already opened!");

		sem = _sem;
		zipfile = project_file;
		DeCompress();

		//saving the original DTDPath
		//and setting to the project's temporary directory
		string saved_dtd_path = UdmDom::DomDataNetwork::DTDPath;
		UdmDom::DomDataNetwork::DTDPath = temp_path;


		//open _project_ DataNetwork ... this will contain all the other DataNetworks
		string project_path = temp_path + PATHDELIM + "_project_.xml";
		//Udm::SmartDataNetwork dn(::UdmProject::diagram);

		project_dn = new Udm::SmartDataNetwork(::UdmProject::diagram);
		project_dn->OpenExisting(project_path, "UdmProject.xsd", Udm::CHANGES_PERSIST_ALWAYS);
		Project = project_dn->GetRootObject();

		::UdmProject::Project _project = ::UdmProject::Project::Cast(Project);

		::UdmProject::Datanetwork cross_ass = _project.cross_associations();
		if(cross_ass)
		{
			string systemname = cross_ass.systemname();
			string systempath = temp_path + PATHDELIM + systemname;
			cross_meta_dn = new SmartDataNetwork(Uml::diagram);
			cross_meta_dn->OpenExisting(systempath, "Uml", Udm::CHANGES_LOST_DEFAULT);
			cross_meta_dn_sem = Udm::CHANGES_LOST_DEFAULT;
		}
		
		::UdmProject::Datanetwork cross_links_pdn = _project.crosslinks();
		if(cross_links_pdn)
		{
			string systemname = cross_links_pdn.systemname();
			string systempath = temp_path + PATHDELIM + systemname;
			//const UdmDiagram & meta = Udm::MetaDepository::LocateDiagram(cross_links_pdn.metaDgr());
			const UdmDiagram & meta = LocateDiagram(cross_links_pdn.metaDgr());
			cross_meta = const_cast<UdmDiagram*>(&meta);
			cross_links = new SmartDataNetwork(meta);
			cross_links->OpenExisting(systempath, cross_links_pdn.metaDgr(), Udm::CHANGES_LOST_DEFAULT);
		}

		// Needed by gcc
		typedef ::UdmProject::Datanetwork UdmPrDn;

		vector<UdmPrDn> dns = _project.instances();
		vector<UdmPrDn>::iterator i = dns.begin();
		while (i != dns.end())
		{
			string systemname = i->systemname();
			string systempath = temp_path + PATHDELIM + systemname;
			//Udm::SmartDataNetwork * dn = new SmartDataNetwork(Udm::MetaDepository::LocateDiagram(i->metaDgr()), this);
			Udm::SmartDataNetwork * dn = new SmartDataNetwork(LocateDiagram(i->metaDgr()), this);
			dn->OpenExisting(systempath, i->metaDgr(), sem);
			datanetworks[systemname] = dn->testdn();
			rev_datanetworks[dn->testdn()] = systemname;

			i++;
		}



		//restoring DTD path 
		UdmDom::DomDataNetwork::DTDPath = saved_dtd_path;
	}



	UDM_DLL vector<Udm::DataNetwork*> UdmProject::GetDataNetworks()
	{
		vector<Udm::DataNetwork*> ret;
		for (map<string, DataNetwork*>::iterator i = datanetworks.begin(); i != datanetworks.end(); i++)
			ret.push_back(i->second);

		return ret;
	};

	UDM_DLL vector< ::Uml::Diagram> UdmProject::GetMetaDiagrams() const
	{
		vector< ::Uml::Diagram> ret;
		for (map<string, DataNetwork*>::const_iterator i = datanetworks.begin(); i != datanetworks.end(); i++)
			ret.push_back(i->second->GetRootMeta());

		return ret;

	};

	UDM_DLL bool UdmProject::IsCrossMetaSpecifier() const
	{
		return (cross_meta_dn != NULL);
	};

	UDM_DLL bool UdmProject::HasCrossMeta() const
	{
		return (cross_meta != NULL);
	};


	UDM_DLL DataNetwork& UdmProject::GetCrossMetaNetwork()
	{
		if (cross_meta_dn == NULL)
			throw udm_exception("CrossMeta Network is NULL!");
		return *cross_meta_dn;
	};
		

	UDM_DLL UdmDiagram& UdmProject::GetCrossMeta()	const
	{
		if (cross_meta == NULL)
			throw udm_exception("CrossMeta Network is NULL!");

		return *cross_meta;
	};


	UDM_DLL UdmProject::~UdmProject()
	{
		
		
		if (zipfile.size())
		{
			//zipFile was opened
			//zip it back
			//remove temporary files

			if (Project)
			{
				//cross links data network
				if (cross_links )
				{
					if (sem != Udm::CHANGES_LOST_DEFAULT)
						cross_links->CloseWithUpdate();
					else cross_links->CloseNoUpdate();


					delete cross_links;
					cross_links = NULL;
				}

				map<string, Udm::DataNetwork*>::iterator i = datanetworks.begin();

				while (i != datanetworks.end())
				{
					if (sem != Udm::CHANGES_LOST_DEFAULT)
						i->second->CloseWithUpdate();
					else
						i->second->CloseNoUpdate();
					
					delete i->second;
					i->second = NULL;
					i++;
				}

				datanetworks.clear();
				rev_datanetworks.clear();

				Project = Udm::null;

				//cross meta data network

				if (cross_meta_dn)
				{
					if (cross_meta_dn_sem != Udm::CHANGES_LOST_DEFAULT)
						cross_meta_dn->CloseWithUpdate();
					else
						cross_meta_dn->CloseNoUpdate();

					delete cross_meta_dn;
					cross_meta_dn = NULL;
				}

				//project data network
				if (!project_dn)
					throw udm_exception("UdmProject::Save(): Project datanetwork is null!");
				if (sem != Udm::CHANGES_LOST_DEFAULT)
					project_dn->CloseWithUpdate();
				else project_dn->CloseNoUpdate();

				delete project_dn;
				project_dn = NULL;



			}
			Compress();

			zipfile = "";
		}

		//at this point the temp_dir should be empty
#ifdef WIN32
		_rmdir(temp_path.c_str());
#else	//WIN32
		rmdir(temp_path.c_str());
#endif	//WIN32


	};

	void UDM_DLL UdmProject::CreateNew(const string &pr_file, vector<DataNetworkSpecifier> spec, const Udm::UdmDiagram& cross_diag, enum BackendSemantics _sem)
	{
		//saving the original DTDPath
		//and setting to the project's temporary directory
		string saved_dtd_path = UdmDom::DomDataNetwork::DTDPath;
		UdmDom::DomDataNetwork::DTDPath = temp_path;

		sem = _sem;
		if (zipfile.size())
			throw udm_exception(string("UdmProject error: Project already opened:") + zipfile);

		if (Project || project_dn)
			throw udm_exception("UdmProject error: Project already opened. Project object exists");

		if (!temp_path.size())
			throw udm_exception("UdmProject error: Temporary folder is not set!");

		//create UdmProject::Project (this will initialize the UdmProject meta-diagram)
		project_dn = new Udm::SmartDataNetwork(::UdmProject::diagram);

		//generate UdmProject.dtd into the project(this requires the Project meta to be initialized)
		ofstream ff;
		ff.open(string(temp_path + PATHDELIM + "UdmProject.xsd").c_str());
		if(!ff.good()) throw udm_exception("Error opening for write UdmProject.dtd");
		else DTDGen::GenerateXMLSchema(*::UdmProject::diagram.dgr, ff);
		ff.close();
		ff.clear();


		//this requires the above generated DTD
		project_dn->CreateNew(temp_path + PATHDELIM + "_project_.xml", "UdmProject",::UdmProject::Project::meta,sem);
		Project = project_dn->GetRootObject();
		::UdmProject::Project _project = ::UdmProject::Project::Cast(Project);

		

		
		_project.name() = pr_file.substr(0, pr_file.find('.'));

		zipfile = pr_file;
		vector<DataNetworkSpecifier>::iterator i = spec.begin();
		while (i != spec.end())
		{	

			//this will initialize the meta diagram
			//Udm::SmartDataNetwork * dn = new SmartDataNetwork(Udm::MetaDepository::LocateDiagram(i->metalocator), this);
			Udm::SmartDataNetwork * dn = new SmartDataNetwork(LocateDiagram(i->metalocator()), this);
			//generate the corresponding DTD( this needs the diagram to be initialized)
			::Uml::Diagram *dgr = LocateDiagram(i->metalocator()).dgr;
			ff.open(string(temp_path + PATHDELIM + (string)dgr->name() + ".xsd").c_str());
			if (!ff.good()) throw udm_exception("Error opening for write DTD file");
			else DTDGen::GenerateXMLSchema(*dgr, ff);
			ff.close();
			ff.clear();

			set< ::Uml::Namespace> nses = dgr->namespaces();
			for(set< ::Uml::Namespace>::iterator nses_i = nses.begin(); nses_i != nses.end(); nses_i++)
			{
				::Uml::Namespace ns = *nses_i;
				ff.open(string(temp_path + PATHDELIM + (string)ns.name() + ".xsd").c_str());
				if(!ff.good()) throw udm_exception("Error opening for write DTD file");
				//else DTDGen::GenerateDTD(*(Udm::MetaDepository::LocateDiagram(i->metalocator).dgr), ff);
				else DTDGen::GenerateXMLSchema(ns, ff);
				ff.close();
				ff.clear();
			}
			
			//create the data network (this needs the DTD)
			dn->CreateNew(temp_path + PATHDELIM + i->filename(), i->metalocator(), i->rootclass(),sem);
			
			datanetworks.insert(pair<string, Udm::DataNetwork*>(i->filename(), dn->testdn()));
			rev_datanetworks.insert(pair<Udm::DataNetwork*, string>(dn->testdn(), i->filename()));
			dn->release();
			delete dn;
			
			::UdmProject::Datanetwork _udm_pr_dn = ::UdmProject::Datanetwork::Create(_project,::UdmProject::Project::meta_instances);
			_udm_pr_dn.metalocator() = i->metalocator();
			_udm_pr_dn.systemname() = i->filename();
			//_udm_pr_dn.metaDgr() = (*(Udm::MetaDepository::LocateDiagram(i->metalocator).dgr)).name();
			_udm_pr_dn.metaDgr() = (string)((*(LocateDiagram(i->metalocator()).dgr)).name());
			
			
			i++;
		};

		//create cross-link data network
		cross_links = new Udm::SmartDataNetwork(cross_diag);

		::UdmProject::Datanetwork _udm_pr_dn = ::UdmProject::Datanetwork::Create(_project,::UdmProject::Project::meta_crosslinks);
		_udm_pr_dn.metalocator() = (string)(cross_diag.dgr->name());
		_udm_pr_dn.systemname() = (string)(_project.name()) + ".xml";
		_udm_pr_dn.metaDgr() = (string)(cross_diag.dgr->name());

		cross_meta = const_cast<Udm::UdmDiagram*>(&cross_diag);

		ff.open(string(temp_path + PATHDELIM + (string)(cross_diag.dgr->name()) + ".xsd").c_str());
		if(!ff.good()) throw udm_exception("Error opening for write XSD file");
		else DTDGen::GenerateXMLSchema(*cross_diag.dgr, ff);
		ff.close();
		ff.clear();
			
		cross_links->CreateNew(temp_path + PATHDELIM + (string)(_udm_pr_dn.systemname()), _udm_pr_dn.metalocator(), Uml::classByName(*cross_diag.dgr, "_gen_cont"), Udm::CHANGES_PERSIST_ALWAYS);
		
		//restoring DTD path 
		UdmDom::DomDataNetwork::DTDPath = saved_dtd_path;	
	};

	void UDM_DLL UdmProject::CreateNewMeta(const string &project_name, const string &pr_file, vector<DataNetworkSpecifier> spec, enum BackendSemantics _sem)
	{
		//saving the original DTDPath
		//and setting to the project's temporary directory
		string saved_dtd_path = UdmDom::DomDataNetwork::DTDPath;
		UdmDom::DomDataNetwork::DTDPath = temp_path;

		sem = _sem;
		if (zipfile.size())
			throw udm_exception(string("UdmProject error: Project already opened:") + zipfile);

		if (Project || project_dn)
			throw udm_exception("UdmProject error: Project already opened. Project object exists");

		if (!temp_path.size())
			throw udm_exception("UdmProject error: Temporary folder is not set!");

		//create UdmProject::Project (this will initialize the UdmProject meta-diagram)
		project_dn = new Udm::SmartDataNetwork(::UdmProject::diagram);

		//generate UdmProject.dtd into the project(this requires the Project meta to be initialized)
		ofstream ff;
		ff.open(string(temp_path + PATHDELIM + "UdmProject.xsd").c_str());
		if(!ff.good()) throw udm_exception("Error opening for write UdmProject.dtd");
		else DTDGen::GenerateXMLSchema(*::UdmProject::diagram.dgr, ff);
		ff.close();
		ff.clear();


		//this requires the above generated DTD
		project_dn->CreateNew(temp_path + PATHDELIM + "_project_.xml", "UdmProject",::UdmProject::Project::meta,sem);
		Project = project_dn->GetRootObject();
		::UdmProject::Project _project = ::UdmProject::Project::Cast(Project);

		

		
		_project.name() = project_name;

		zipfile = pr_file;
		vector<DataNetworkSpecifier>::iterator i = spec.begin();
		while (i != spec.end())
		{	

			//this will initialize the meta diagram
			//Udm::SmartDataNetwork * dn = new SmartDataNetwork(Udm::MetaDepository::LocateDiagram(i->metalocator), this);
			Udm::SmartDataNetwork * dn = new SmartDataNetwork(LocateDiagram(i->metalocator()), this);
			//generate the corresponding DTD( this needs the diagram to be initialized)
			::Uml::Diagram *dgr = LocateDiagram(i->metalocator()).dgr;
			ff.open(string(temp_path + PATHDELIM + (string)dgr->name() + ".xsd").c_str());
			if (!ff.good()) throw udm_exception("Error opening for write DTD file");
			else DTDGen::GenerateXMLSchema(*dgr, ff);
			ff.close();
			ff.clear();

			set< ::Uml::Namespace> nses = dgr->namespaces();
			for(set< ::Uml::Namespace>::iterator nses_i = nses.begin(); nses_i != nses.end(); nses_i++)
			{
				::Uml::Namespace ns = *nses_i;
				ff.open(string(temp_path + PATHDELIM + (string)ns.name() + ".xsd").c_str());
				if(!ff.good()) throw udm_exception("Error opening for write DTD file");
				//else DTDGen::GenerateDTD(*(Udm::MetaDepository::LocateDiagram(i->metalocator).dgr), ff);
				else DTDGen::GenerateXMLSchema(ns, ff);
				ff.close();
				ff.clear();
			}
			
			//create the data network (this needs the DTD)
			dn->CreateNew(temp_path + PATHDELIM + i->filename(), i->metalocator(), i->rootclass(),sem);
			
			datanetworks.insert(pair<string, Udm::DataNetwork*>(i->filename(), dn->testdn()));
			rev_datanetworks.insert(pair<Udm::DataNetwork*, string>(dn->testdn(), i->filename()));
			
			
			::UdmProject::Datanetwork _udm_pr_dn = ::UdmProject::Datanetwork::Create(_project,::UdmProject::Project::meta_instances);
			_udm_pr_dn.metalocator() = i->metalocator();
			_udm_pr_dn.systemname() = i->filename();
			//_udm_pr_dn.metaDgr() = (*(Udm::MetaDepository::LocateDiagram(i->metalocator).dgr)).name();
			_udm_pr_dn.metaDgr() = (string)((*(LocateDiagram(i->metalocator()).dgr)).name());
			
			
			i++;
		};

		//create cross meta data network
		cross_meta_dn = new Udm::SmartDataNetwork(::Uml::diagram);
		cross_meta_dn_sem = Udm::CHANGES_PERSIST_ALWAYS;

		::UdmProject::Datanetwork _udm_pr_dn = ::UdmProject::Datanetwork::Create(_project,::UdmProject::Project::meta_cross_associations);
		_udm_pr_dn.metalocator() = "Uml";
		_udm_pr_dn.systemname() = project_name + ".xml";
		_udm_pr_dn.metaDgr() = (string)((*(LocateDiagram("Uml").dgr)).name());

		cross_meta_dn->CreateNew(temp_path + PATHDELIM + (string)(_udm_pr_dn.systemname()), _udm_pr_dn.metalocator(), ::Uml::Diagram::meta, Udm::CHANGES_PERSIST_ALWAYS);

		::Uml::Diagram cross_meta_dgr = ::Uml::Diagram::Cast(cross_meta_dn->GetRootObject());
		cross_meta_dgr.name() = project_name;
		cross_meta_dgr.version() = "1.00";

		::Uml::Class cont_class = ::Uml::Class::Create(cross_meta_dgr);
		cont_class.name() = "_gen_cont";
		cont_class.isAbstract() = false;

		ff.open(string(temp_path + PATHDELIM + "Uml.xsd").c_str());
		if(!ff.good()) throw udm_exception("Error opening for write XSD file");
		else DTDGen::GenerateXMLSchema(*Uml::diagram.dgr, ff);
		ff.close();
		ff.clear();

		//restoring DTD path 
		UdmDom::DomDataNetwork::DTDPath = saved_dtd_path;	
	};

	UDM_DLL Udm::DataNetwork&  UdmProject::GetDataNetwork(const string &which)
	{
		map<string, Udm::DataNetwork*>::iterator i = datanetworks.find(which);
		if (i != datanetworks.end())
			return *(i->second);

		throw udm_exception(string("Project ") + which + " is not part of the project!");
		
	};


	Udm::Object UdmProject::GetPlaceHolder(const Udm::Object& o, bool create)
	{
		if (!Project) throw udm_exception("Project:Object handle is null!");

		if (!cross_meta) throw udm_exception("Project meta-information is null!");

		if (!cross_links) throw udm_exception("Cross-link datanetwork is null!");

		//::UdmProject::Project pr = ::UdmProject::Project::Cast(Project);
		//Uml::Class gen_cont_class = Uml::classByName(*(cross_meta->dgr), "_gen_cont");

		Udm::Object root_object = cross_links->GetRootObject();
		if(!root_object) 
			throw udm_exception("Root Object for cross_link data network is null!");

		//if o is null, return null 
		// this is needed because of setting associations to null has a meaning - deleting links
		if(!o) return o;

		::Uml::Class ph_class = Uml::GetClassFromCrossDgr(*(cross_meta->dgr), o.type());

		set<Udm::Object> ph_children = root_object.GetChildObjects(ph_class);
		set<Udm::Object>::iterator ph_c_i = ph_children.begin();

		while (ph_c_i != ph_children.end())
		{
			Udm::Object ph_object = *ph_c_i;
			string ph_sys_name;


			if (!(ph_object.GetStrValue("rem_sysname", ph_sys_name)))
				throw udm_exception("GetIntValue() failed for attribute rem_sysname");

			map<string, Udm::DataNetwork*>::iterator rev_i = datanetworks.find(ph_sys_name);
			if (rev_i == datanetworks.end())
				throw udm_exception(string("Datanetwork with systemname '") + ph_sys_name + string("' was not found in the reverse map!"));

			if (rev_i->second == o.__impl()->__getdn())
			{
				__int64 ph_object_id;
			
				if (!(ph_object.GetIntValue("rem_id", ph_object_id)))
					throw udm_exception("GetIntValue() failed for attribute rem_id");
				if (ph_object_id == o.__impl()->uniqueId())
					return ph_object;
			}
			ph_c_i++;
		};

		if (!create) return Udm::null;
		//placeholder was not found ... create it
		// Due to inheritance, an object may have multiple roles. CreateObject will fail if it is ambiguous.
		// So just get any valid child role
		Udm::Object::CompositionInfo compInfo;
		{
			// like matchChildToParent, but dups are fine
			using namespace Uml;
			set<Class> pancs = AncestorClasses(root_object.type());
			set<Class> cancs = AncestorClasses(ph_class);
			for (set<Class>::iterator j = cancs.begin(); j != cancs.end(); j++) {
				set<CompositionChildRole> cr = (*j).childRoles();
				for (set<CompositionChildRole>::iterator i = cr.begin(); i != cr.end(); i++) {
					if (pancs.find(theOther(*i).target()) != pancs.end()) {
						Uml::Composition comp;
						comp = (*i).parent();
						compInfo.strChildRoleName = i->name();
						compInfo.strParentRoleName = theOther(*i).name();
					}
				}
			}
		}
		Object ph_object = root_object.CreateObject(Uml::SafeTypeContainer::GetSafeType(ph_class), compInfo);
		if(!ph_object.SetIntValue("rem_id", o.__impl()->uniqueId()))
			throw udm_exception("SetIntValue failed when setting attribute rem_id!");

		if(!ph_object.SetStrValue("rem_sysname", rev_datanetworks[o.__impl()->__getdn()]))
			throw udm_exception("SetStrValue failed when setting attribute rem_sysname!");

		return ph_object;


	};

	Udm::Object UdmProject::GetRealObject(const Udm::Object& ph)
	{
		string rem_sysname;
		__int64 rem_id;

		if(!ph.GetStrValue("rem_sysname", rem_sysname))
			throw udm_exception("GetStrValue failed when getting attribute rem_sysname on a placeholder object!");
		if(!ph.GetIntValue("rem_id", rem_id))
			throw udm_exception("GetIntValue failed when getting attribute rem_id on a placeholder object!");

		map<string, Udm::DataNetwork*>::iterator i = datanetworks.find(rem_sysname);
		if (i == datanetworks.end())
			throw udm_exception(string("Lookup of systemname failed in datanetwork map: ") + rem_sysname);

		Udm::DataNetwork* rem_dn = i->second;
		if (rem_dn == NULL)
			throw udm_exception("Datanetwork handle is null!");
		return rem_dn->ObjectById((long)rem_id);//explicit cast to avoid warning 


	};

	void UDM_DLL UdmProject::setSimpleAssociation(Udm::Object& peer, set<Udm::Object>& peers,const ::Uml::AssociationRole& meta)
	{
		//obtain a proxy for this side
		Udm::Object peer_ph = GetPlaceHolder(peer);

		//obtain a proxy for each object on the other side
		set<Udm::Object> peers_ph;
		set<Udm::Object>::iterator i = peers.begin();
		while (i != peers.end())
		{
			Udm::Object peers_ph_o = GetPlaceHolder(*i);
			if (!peers_ph_o && (*i))
					throw udm_exception("UdmProject::setSimpleAssociation(): GetPlaceHolder(link) failed!");
				
			if(peers_ph_o) 
				peers_ph.insert(peers_ph_o);
			i++;
		};

		//set the association between the proxies
		peer_ph.setAssociation(meta, peers_ph);	
	};

	UDM_DLL set<Udm::Object> UdmProject::getSimpleAssociation(Udm::Object& peer,const ::Uml::AssociationRole& meta)
	{
		//obtain a proxy for this side (probably it exists
		Udm::Object peer_ph = GetPlaceHolder(peer, false);
		set<Udm::Object> peers_ph;
		set<Udm::Object> ret;

		//obtain the associated proxies
		if (peer_ph)
			peers_ph = peer_ph.getAssociation(meta);
		
		//do the translation into real objects
		set<Udm::Object>::iterator i = peers_ph.begin();
		while (i != peers_ph.end())
		{
			Udm::Object peers_o = GetRealObject(*i);
			ret.insert(peers_o);
			i++;
		};
		
		//return 
		return ret;
	};

	void UDM_DLL UdmProject::setLinks(Udm::Object& peer, set<Udm::Object>& links, const ::Uml::AssociationRole& meta)
	{
		//forward the call with CLASSFROMTARGET
		//obtain a proxy for this side (probably it exists
		Udm::Object peer_ph = GetPlaceHolder(peer);
		
		//obtain the associated proxies
		if (peer_ph)
		{
			set<Udm::Object> links_ph;

			set<Udm::Object>::iterator i = links.begin();
			while (i != links.end())
			{
				Udm::Object link_ph = GetPlaceHolder(*i);
				if (!link_ph && (*i))
					throw udm_exception("UdmProject::setLinks(): GetPlaceHolder(link) failed!");
				if (link_ph)
					links_ph.insert(link_ph);
				i++;
			}

			//setting the association between the proxies
			peer_ph.setAssociation(meta, links_ph, Udm::CLASSFROMTARGET);
		}
		else
			throw udm_exception("UdmProject::setLinks(): GetPlaceHolder(peer) failed!");

	
	};

	void UDM_DLL UdmProject::setEnd(Udm::Object& link, Udm::Object& peer, const ::Uml::AssociationRole& meta)
	{
		//forward the call with TARGETFROMCLASS
		Udm::Object link_ph = GetPlaceHolder(link);
		if (link_ph)
		{
			
			Udm::Object peer_ph = GetPlaceHolder(peer);
			if (peer_ph || !peer)
			{
				set<Udm::Object> peers_ph;
				if (peer_ph)
					peers_ph.insert(peer_ph);
				link_ph.setAssociation(meta, peers_ph, Udm::TARGETFROMCLASS);

			}
			else
				throw udm_exception("UdmProject::setEnd(): GetPlaceHolder(peer) failed!");
		}
		else
			throw udm_exception("UdmProject::setEnd(): GetPlaceHolder(link) failed!");
	};


	UDM_DLL set<Udm::Object> UdmProject::getLinks(Udm::Object& peer, const ::Uml::AssociationRole &meta)
	{
		//forward the call with CLASSFROMTARGET
		
		//obtain a proxy for this side (probably it exists
		Udm::Object peer_ph = GetPlaceHolder(peer, false);
		set<Udm::Object> peers_ph;
		set<Udm::Object> ret;

		//obtain the associated proxies
		if (peer_ph)
			peers_ph = peer_ph.getAssociation(meta, Udm::CLASSFROMTARGET);
		
		//do the translation into real objects
		set<Udm::Object>::iterator i = peers_ph.begin();
		while (i != peers_ph.end())
		{
			Udm::Object peers_o = GetRealObject(*i);
			ret.insert(peers_o);
			i++;
		};
		
		//return 
		return ret;
	};

	UDM_DLL Udm::Object UdmProject::getEnd(Udm::Object& link, const ::Uml::AssociationRole &meta)
	{
		//forward the call with TARGETFROMCLASS
		//obtain a proxy for this side (probably it exists
		Udm::Object link_ph = GetPlaceHolder(link, false);
		set<Udm::Object> peers_ph;
		Udm::Object ret;

		//obtain the associated proxies
		if (link_ph)
			peers_ph = link_ph.getAssociation(meta, Udm::TARGETFROMCLASS);
		
		//do the translation into real objects
		if (peers_ph.size()>1)
			throw udm_exception("UdmProject::getEnd(): expected <= 1 peer!");

		if (peers_ph.begin() != peers_ph.end())
			ret = GetRealObject(*(peers_ph.begin()));
		
		//return 
		return ret;
	};
		

	UDM_DLL Udm::Object UdmProject::getPeer(Udm::Object& peer, const ::Uml::AssociationRole &meta)
	{
		//forward the call with TARGETFROMPEER

		//obtain a proxy for this side (probably it exists
		Udm::Object peer_ph = GetPlaceHolder(peer, false);
		set<Udm::Object> peers_ph;
		Udm::Object ret;

		//obtain the associated proxies
		if (peer_ph)
			peers_ph = peer_ph.getAssociation(meta, Udm::TARGETFROMPEER);
		
		//do the translation into real objects
		if (peers_ph.size()>1)
			throw udm_exception("UdmProject::getEnd(): expected <= 1 peer!");

		if (peers_ph.begin() != peers_ph.end())
			ret = GetRealObject(*(peers_ph.begin()));
		
		//return 
		return ret;

	};

	
	void UDM_DLL UdmProject::ValidateIDs(const string &sys_name, const map<const unsigned long, const unsigned long> &tr_map)
	{
		//only projects that have a cross-link datanetwork apply
		if (cross_links == NULL)
			throw udm_exception("UdmProject::ValidateIDs(): Project does not contain a  cross-link datanetwork to validate!");


		
		Udm::Object ro = cross_links->GetRootObject();

		//no need for recursion - by design, the cross-link datanetwork will have no hierarchy:
		//the rootobject of type _gen_cont, and the placeholder children

		set<Udm::Object> children = ro.GetChildObjects();
		for(set<Udm::Object>::iterator ch_i = children.begin();ch_i != children.end();ch_i++)
		{
			Udm::Object ch_i_o = *ch_i;	
			string val;
			
			if (ch_i_o.GetStrValue( "rem_sysname", val))
			{
				val  = temp_path + PATHDELIM + val;
				if (val == sys_name)
				{
					__int64 int_val;
					if(ch_i_o.GetIntValue("rem_id", int_val))
					{
						const unsigned long ulint_val = (const unsigned long)int_val;
						map<const unsigned long, const unsigned long>::const_iterator ci = tr_map.find(ulint_val);
						if (ci != tr_map.end())
						{
							if (!ch_i_o.SetIntValue("rem_id", ci->second))
								throw udm_exception("UdmProject::ValidateIDs(): SetIntValue(rem_id) failed for a placeholder object!");
						}
						else
							throw udm_exception("UdmProject::ValidateIDs(): No record in tr_map was found for a placeholder object!");
					}
					else
						throw udm_exception("UdmProject::ValidateIDs(): GetIntValue(rem_id) failed for a placeholder object!");
				};
				//no else branch here, this is not an error
				//the object perhaps is a placeholder object from another datanetwork
			}
			else
				throw udm_exception("UdmProject::ValidateIDs(): GetStrValue(rem_sysname) failed for a placeholder object!");

		};



	};

	const  UdmDiagram & UdmProject::LocateDiagram(const string& DgrName)
	{
		map<const string, const Udm::UdmDiagram *>::iterator i = dms.find(DgrName);
		if (i != dms.end())
			return *(i->second);

		//use the static MetaDepository
		//(i.e. rely on the compiled-in .cpp metas)
		return MetaDepository::LocateDiagram(DgrName);
	};

	/* Dynamic meta support */
	//add dynamic meta specifier
	void UDM_DLL UdmProject::AddDynamicMeta(const DynamicMetaSpecifier& ds)
	{
		dms[ds.metalocator] = &(ds.dgr);
	};

	//reset dynamic meta specifiers
	void UDM_DLL UdmProject::ResetDynamicMetas()
	{
		dms.erase(dms.begin(), dms.end());
	};


	void UDM_DLL StaticUdmProject::Close() // just a normal clean-up, no persistence issues
	{
		if (project_dn)
		{
			map<string, DataNetwork*>::iterator i = datanetworks.begin();
				while ( i != datanetworks.end())
				{
					i->second->resetStaticUdmProject();
					i++;
				};

				datanetworks.clear();
				rev_datanetworks.clear();

				Project = Udm::null;

				//cross meta data network

				if (cross_links)
				{
					cross_links->CloseNoUpdate();//we never save this
					delete cross_links;
					cross_links = NULL;
				}

				//project data network
				if (!project_dn)
					throw udm_exception("UdmProject::Save(): Project datanetwork is null!");
				
				project_dn->CloseNoUpdate();

				delete project_dn;
				project_dn = NULL;
		};

	};

	UDM_DLL StaticUdmProject::~StaticUdmProject()		
	{
		//we Close() it, if the user hasn't
		Close();
	};


	UDM_DLL StaticUdmProject::StaticUdmProject(vector<StaticDataNetworkSpecifier> sdns, const Udm::UdmDiagram& cross_diag)
		: UdmProject(true)//it's a static network, UdmProject() should not create temp dirs and stuff
	{

		//creating a non-persistent project data network
		project_dn = new UdmStatic::StaticDataNetwork(::UdmProject::diagram);
		project_dn->CreateNew(" ", " ", ::UdmProject::Project::meta, CHANGES_LOST_DEFAULT);
		Project = project_dn->GetRootObject();
		::UdmProject::Project _project = ::UdmProject::Project::Cast(Project);


		//creating a non-persistent cross-link data network 
		cross_meta = const_cast<Udm::UdmDiagram*>(&cross_diag);
		cross_links = new UdmStatic::StaticDataNetwork(cross_diag);
//		const ::Uml::Class& root_cls = cross_diag.dgr->__impl()->__getdn()->IsTypeSafe() 
//			? ::Uml::classByName(*cross_diag.dgr, "_gen_cont")
//			: Uml::SafeTypeContainer::GetSafeType(::Uml::classByName(*cross_diag.dgr, "_gen_cont"));
		const ::Uml::Class& root_cls = Uml::SafeTypeContainer::GetSafeType(::Uml::classByName(*cross_diag.dgr, "_gen_cont"));
		cross_links->CreateNew(" ", " ", root_cls, CHANGES_LOST_DEFAULT);
	

		//adding the cross-link data network to the project
		::UdmProject::Datanetwork _udm_pr_dn = ::UdmProject::Datanetwork::Create(_project,::UdmProject::Project::meta_crosslinks);
		/* //these can be left out
		_udm_pr_dn.metalocator() = cross_diag.dgr->name();
		_udm_pr_dn.systemname() = i->filename;
		_udm_pr_dn.metaDgr() = i->datanetwork->GetRootMeta()->name();
		*/
		

		//adding the datanetworks to the project
		vector<StaticDataNetworkSpecifier>::iterator i = sdns.begin();
		while (i != sdns.end())
		{
			/*
			//adding to internal maps
			datanetworks[i->filename] = i->datanetwork->testdn();
			rev_datanetworks[i->datanetwork->testdn()] = i->filename;
			*/
	
			datanetworks[i->filename()] = i->GetDataNetwork();
			rev_datanetworks[i->GetDataNetwork()] = i->filename();
			

			//adding it to the project
			
			::UdmProject::Datanetwork _udm_pr_dn = ::UdmProject::Datanetwork::Create(_project,::UdmProject::Project::meta_instances);
			/* // - these can be left out
			_udm_pr_dn.metalocator() = i->datanetwork->GetRootMeta()->name();
			_udm_pr_dn.systemname() = i->filename;
			_udm_pr_dn.metaDgr() = i->datanetwork->GetRootMeta()->name();
			*/
			//i->datanetwork->setStaticUdmProject(this);
			i->GetDataNetwork()->setStaticUdmProject(this);

			i++;
		};


	};


	udm_exception _e("Method not defined for StaticUdmProject!");
	//all of these will throw an exception because they are not suitable for Static projects
	UDM_DLL StaticUdmProject::StaticUdmProject()
	{
		throw (_e);
	};

	UDM_DLL StaticUdmProject::StaticUdmProject(const string & project_file, vector<DataNetworkSpecifier>, const Udm::UdmDiagram& cross_diag , enum BackendSemantics)
	{
		throw(_e);
	};

	void UDM_DLL StaticUdmProject::OpenExisting(const string & project_file, enum BackendSemantics)
	{
		throw(_e);
	};

	void UDM_DLL StaticUdmProject::ValidateIDs(const string& sysname, const map<const unsigned long, const unsigned long> &tr_map)
	{
		throw(_e);
	};

	void UDM_DLL StaticUdmProject::AddDynamicMeta(const DynamicMetaSpecifier&)
	{
		throw(_e);
	};

	void UDM_DLL StaticUdmProject::ResetDynamicMetas()
	{
		throw(_e);
	};

	UDM_DLL DataNetworkSpecifier::DataNetworkSpecifier( string fn,  string ml, ::Uml::Class& rc)
			: _filename(fn), _metalocator(ml), _rootclass(rc)
	{
	};

	UDM_DLL DataNetworkSpecifier& DataNetworkSpecifier::operator=(const DataNetworkSpecifier& a)
	{
		if (this == &a) return *this;
		_filename = a._filename;
		_metalocator = a._metalocator;
		_rootclass = a._rootclass;
		return *this;
	};
	UDM_DLL const string DataNetworkSpecifier::filename()
	{
		return _filename;
	};
	UDM_DLL const string DataNetworkSpecifier::metalocator()
	{
		return _metalocator;
	};
	UDM_DLL const ::Uml::Class& DataNetworkSpecifier::rootclass()
	{
		return _rootclass;
	};

	UDM_DLL DataNetwork* StaticDataNetworkSpecifier::GetDataNetwork() 
	{ 
		return simpledatanetwork ? simpledatanetwork : datanetwork->testdn(); 
	};	

	UDM_DLL StaticDataNetworkSpecifier::StaticDataNetworkSpecifier(string fn, SmartDataNetwork * dn)
			: _filename(fn), datanetwork(dn), simpledatanetwork(NULL) 
	{
	};

	UDM_DLL StaticDataNetworkSpecifier::StaticDataNetworkSpecifier(string fn, DataNetwork * dn)
			: _filename(fn), datanetwork(NULL), simpledatanetwork(dn)
	{
	};

	UDM_DLL const string StaticDataNetworkSpecifier::filename()
	{
		return _filename;
	};
};
