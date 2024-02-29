#ifndef MOBIES_Uml_export_H
#	define MOBIES_Uml_export_H
#	ifndef UDM_DLL 
#		if defined (Uml_HAS_DLL) && (Uml_HAS_DLL == 1)
#			if defined (Uml_EXPORT)
#				define UDM_DLL __declspec(dllexport)
#			else
#				define UDM_DLL __declspec(dllimport)
#			endif /* Uml_EXPORT */
#		else
#			define UDM_DLL 
#		endif /* Uml_HAS_DLL */
#	endif /* ifndef UDM_DLL  */
#endif /* MOBIES_Uml_export_H */
