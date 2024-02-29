#!python -u

import sys
import os
import os.path

prefs = { 'verbose': True }

os.environ['PATH'] = os.environ['PATH'].replace('"', '')

def add_wix_to_path():
    import _winreg
    for wix_ver in ('3.5', '3.6', '3.7', '3.8', '3.9', '3.10', '3.11'):
        try:
            with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, 'SOFTWARE\\Microsoft\\Windows Installer XML\\' + wix_ver, 0, _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as wixkey:
                os.environ['PATH'] = _winreg.QueryValueEx(wixkey, 'InstallRoot')[0] + ';' + os.environ['PATH']
        except Exception as e:
            pass

def system(args, dirname=None):
    """
    Executes a system command (throws an exception on error)
    params
        args : [command, arg1, arg2, ...]
        dirname : if set, execute the command within this directory
    """
    import subprocess
    # print args
    with open(os.devnull, "w") as nulfp:
        # n.b. stderr=subprocess.STDOUT fails mysteriously
        import sys
        subprocess.check_call(args, stdout=(sys.stdout if prefs['verbose'] else nulfp), stderr=subprocess.STDOUT, shell=False, cwd=dirname)

def adjacent_file(file):
    return os.path.join(os.path.dirname(__file__), file)

def build_packed(args):
    sourcedir, arch, msi = args
    build(sourcedir, arch, msi)

def build(sourcedir, arch, msi=False):
    def get_wixobj(file):
        return os.path.splitext(file)[0] + ('_x64' if arch == 'x64' else '') + ".wixobj"

    def get_svnversion():
        import subprocess
        p = subprocess.Popen(['svnversion', '-n', adjacent_file('..')], stdout=subprocess.PIPE)
        out, err = p.communicate()
        if p.returncode:
            raise subprocess.CalledProcessError(p.returncode, 'svnversion')
        if 'M' in out:
            return '1'
            # out = out.replace('M', '')
        # this will crash for switched or sparse checkouts
        return int(out)

    #print get_svnversion()
    sourcedir = adjacent_file(('' if sourcedir == '' else sourcedir + "\\"))
    import glob
    sources = glob.glob(sourcedir + '*.wxi') + glob.glob(sourcedir + '*.wxs')
    sources = [source for source in sources if source.find('Udm_inc.wxi') == -1]
    if len(sources) == 0:
        raise Exception("0 sources found in " + sourcedir)

    defines = [('GREAT_PATH', os.environ.get('GREAT_PATH', 'GREAT_PATH'))]

    for source in sources:
        #arch = [ '-arch', ('x86' if source.find('x64') == -1 else 'x64') ]
        system(['candle', '-nologo'] + ['-d' + d[0] + '=' + d[1] for d in defines ] + ['-arch', arch] + ['-out', get_wixobj(source), source])

    #ignore warning 1055, ICE82 from VC9 merge modules
    if msi:
        print 'Building msi ' + arch
        msi_names = {'x86': 'Udm.msi', 'x64': 'Udm_x64.msi'}
        dirs = [os.path.basename(d) for d in glob.glob(sourcedir + '*') if os.path.isdir(d) and os.path.basename(d) not in ('Release', 'Debug', '.svn')]
        wixlibs = [os.path.join(d, d + '.wixlib') for d in dirs]
        if arch == 'x64':
            wixlibs += [os.path.join(d, d + '_x64.wixlib') for d in dirs]
            wixlibs = [lib for lib in wixlibs if os.path.basename(lib) not in ('UdmDll_x64.wixlib', 'UdmSDK_VC9_x64.wixlib')]
        system(['light', '-nologo', '-sw1055', '-sice:ICE82', '-ext', 'WixNetFxExtension', '-ext', 'WixUIExtension', '-ext', 'WixUtilExtension', '-o', msi_names[arch] ] +
            [get_wixobj(file) for file in sources ] + wixlibs)
    else:
        wixlib_name = [ os.path.splitext(source)[0] for source in sources if source.find('.wxs') != -1 ][0]
        if arch == 'x64':
            wixlib_name = wixlib_name + '_x64'
            sources = [s for s in sources if os.path.basename(s) not in ('include.wxi', 'UIntWizVS.wxi', 'test_UdmPython.wxi', 'GeneTF.wxi', 'GeneTRE.wxi', 'SBML2Ex.wxi', 'UMLOCLExpressionChecker.wxi')]
        wixlib_name = wixlib_name + '.wixlib'
        system(['lit', '-nologo', '-bf', '-ext', 'WixUtilExtension', '-o', wixlib_name] + [ get_wixobj(file) for file in sources ])

if __name__=='__main__':
    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option('-a', '--arch', dest='arch')
    parser.add_option('-m', '--msi', dest='msi', action='store_true')
    options, args = parser.parse_args()

    add_wix_to_path()

    if not options.arch:
        arches = ['x86', 'x64']
    else:
        arches = [ options.arch ]
    
    files = args
    if len(files) == 0 and not options.msi:
        import glob
        import os.path
        files = [os.path.dirname(wxs) for wxs in glob.glob(os.path.join(os.path.dirname(__file__ ), '*/*.wxs'))]
        if len(files) == 0:
            raise Exception("0 sources found in " + os.path.dirname(__file__ ))
    for file in files:
        for arch in arches:
            print 'Building ' + file + ' ' + arch
            build(file, arch)

    if options.msi or len(args) == 0:
        for arch in arches:
            build('', arch, True)
