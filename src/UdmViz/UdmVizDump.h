// UdmVizDump.h: interface for the UdmVizDump class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UDMVIZDUMP_H__72FECFC1_19ED_40A8_BD00_8ACFA3399973__INCLUDED_)
#define AFX_UDMVIZDUMP_H__72FECFC1_19ED_40A8_BD00_8ACFA3399973__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Uml.h"


UDM_USE_DOM			//use the DOM backend
#ifdef _WIN32
UDM_USE_MGA			//use the MGA backend
#endif




class UdmVizDump  
{
public:

	void Dump(Udm::SmartDataNetwork *sdnBackend);
	void VisitObjects(Udm::Object ob, int nDepthLevel=0);
	int  VisitLinks(Udm::ObjectImpl* p_objectImpl);
	void ExtractAttributes(Udm::Object ob, int nDepthLevel);


	UdmVizDump();
	virtual ~UdmVizDump();

	std::set<long> objectIDSet;

	bool bAggregate;
	bool bLinks;

};

#endif // !defined(AFX_UDMVIZDUMP_H__72FECFC1_19ED_40A8_BD00_8ACFA3399973__INCLUDED_)
