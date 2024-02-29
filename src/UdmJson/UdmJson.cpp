
#include <Uml.h>
#include <UdmUtil.h>
#include <UmlExt.h>
#include <json_spirit_writer_template.h>


UDM_USE_DOM
#ifdef _WIN32
UDM_USE_MGA
#endif

#include <string>
void dummy(void) {; }


/*
 json writer options are:
 
 pretty_print = 0x01,   // Add whitespace to format the output nicely.
 
 raw_utf8 = 0x02,       // This prevents non-printable characters from being escapted using "\uNNNN" notation.
                        // Note, this is an extension to the JSON standard. It disables the escaping of
                        // non-printable characters allowing UTF-8 sequences held in 8 bit char strings
                        // to pass through unaltered.
 
 remove_trailing_zeros = 0x04,
                        // outputs e.g. "1.200000000000000" as "1.2"
 single_line_arrays = 0x08,
                        // pretty printing except that arrays printed on single lines unless they contain
                        // composite elements, i.e. objects or arrays
 always_escape_nonascii = 0x10,
                        // all unicode wide characters are escaped, i.e. outputed as "\uXXXX", even if they are
                        // printable under the current locale, ascii printable chars are not escaped
 */

#define CMD_SW_PREFIX "--"
struct cmd_line_option
{
    string desc;
    unsigned int value;
    cmd_line_option(string d, unsigned int v) : desc(d), value(v) {};
    
};

struct cmd_line_options
{
    unsigned int options;
    map <string, cmd_line_option> options_map;
    set <string> parsed_options;
    vector <string> positional_options;
    void add(string sw, string desc, unsigned int value)
    {
        if (!sw.empty())
        {
            string options_sw = string(CMD_SW_PREFIX) + sw;
            options_map.insert(pair<string, cmd_line_option> (sw, cmd_line_option(desc, value)));
        }
    }
    bool parse(int argc, char **argv)
    {
        while (argc > 1)
        {
            
            
            if (strlen(argv[1]) > strlen(CMD_SW_PREFIX) &&  !strncmp(argv[1], CMD_SW_PREFIX, strlen(CMD_SW_PREFIX)))
            {
                //it is a switch
                map <string, cmd_line_option>::iterator i = options_map.find(argv[1]+strlen(CMD_SW_PREFIX));
                if (i != options_map.end())
                {
                    options = options | i->second.value;
                    parsed_options.insert(argv[1]+strlen(CMD_SW_PREFIX));
                } else return false;
                
            }
            else positional_options.push_back(argv[1]);
            argc--;
            argv++;
        }
        return true;
        
    }
    
    bool option_provided(const string& o)
    
    {
        return (parsed_options.find(o) != parsed_options.end());
    }

};

std::ostream& operator<<(std::ostream& os, const cmd_line_options& clo)
{
    os << "Usage: UdmJson [options] <InputDataNetwork> <UmlMetaDiagram> <OutputJson> [<OutputSchema>]" << endl;
    os << "Valid options are : " << endl;
    
    map <string, cmd_line_option>::const_iterator i = clo.options_map.begin();
    for (; i!= clo.options_map.end(); i++)
    {
        os << string(CMD_SW_PREFIX) << i->first << string(":") << i->second.desc << endl;
    }

    return os;
};


int main(int argc, char **argv)
{
    
    cmd_line_options clo;
    
    clo.add ("help", "                  Produce help message",0);
    clo.add ("pretty_print", "          Add whitespace to format the output nicely", json_spirit::pretty_print);
    clo.add ("remove_trailing_zeros", " Outputs e.g. \"1.200000000000000\" as \"1.2\"", json_spirit::remove_trailing_zeros);
    clo.add ("single_line_arrays", "    Pretty printing arrays on single lines unless they contain composite elements, i.e. objects or arrays",json_spirit::single_line_arrays );
    clo.add ("always_escape_nonascii", "All unicode wide characters are escaped, i.e. outputed as \"\\uXXXX\", even if they are printable under the current locale, ascii printable chars are not escaped", json_spirit::always_escape_nonascii);

    
    
    if (!clo.parse(argc, argv))
    {
        cout << "Error: Invalid options. Use --help for usage "<< endl;
        return -1;
    }
    
    if (clo.option_provided("help"))
    {
        cout << clo << endl;
        return 0;
    }
    
    if (clo.positional_options.size() < 3 || clo.positional_options.size() > 4)
    {
        cout << "Error in providing input/meta/output/schema files. Use --help for usage "<< endl;
        return -1;
    }
    
   
    string input_file = clo.positional_options[0];
    string uml_diagram_file = clo.positional_options[1];
    string json_output_file = clo.positional_options[2];
    
    string schema_output_file;
    if( clo.positional_options.size() ==4 )
         schema_output_file = clo.positional_options[3];
    
    
    
	// Loading the meta
    Udm::SmartDataNetwork  ddnMeta(::Uml::diagram);
    ::Uml::Diagram theUmlDiagram;
    try {

            // Opening the XML meta of the host graph
            Udm::SmartDataNetwork ddnMeta_in(::Uml::diagram);
            ddnMeta_in.OpenExisting(uml_diagram_file,"uml.dtd", Udm::CHANGES_LOST_DEFAULT);
		
            // Casting the DataNetwork to diagram
            theUmlDiagram = ::Uml::Diagram::Cast(ddnMeta_in.GetRootObject());

            // Creating the UDM diagram
            Udm::UdmDiagram udmDataDiagram;
            udmDataDiagram.dgr = &theUmlDiagram;
            udmDataDiagram.init = dummy;

            Udm::SmartDataNetwork DN(udmDataDiagram);
            DN.OpenExisting(input_file,"",Udm::CHANGES_LOST_DEFAULT);
            ::UdmUtil::write_json(DN.GetRootObject(), json_output_file, clo.options);
            DN.CloseNoUpdate();
        
            if (!schema_output_file.empty())
            {
                ::UdmUtil::write_json(theUmlDiagram, schema_output_file, clo.options );
            }
	}
	catch(udm_exception u)
	{
                cerr << u.what() << endl;
                exit(-1);
        }
	
	return 0;

}

