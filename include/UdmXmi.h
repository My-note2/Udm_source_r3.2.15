
#ifndef MOBIES_UDM_XMI_H
#define MOBIES_UDM_XMI_H

#include "ErrHand.h"

void initXSLTstr();
int do_xslt_trans(const char * file_in, const char * file_out, const char * file_xsl);

int XmiToUdm(const char * in, const char * out);
int UdmToXmi(const char * in, const char * out);


#endif
