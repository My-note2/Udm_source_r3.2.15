import sys
if sys.version_info[0:2] == (2,6):
    from Python26.udm import *
elif sys.version_info[0:2] == (2,7):
    from Python27.udm import *
elif sys.version_info[0:2] == (3,3):
    from Python33.udm import *
else:
    raise ImportError("Unsupported Python version " + str(sys.version_info))
