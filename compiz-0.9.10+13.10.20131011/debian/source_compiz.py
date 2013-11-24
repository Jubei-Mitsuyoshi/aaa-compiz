import apport.packaging
from apport.hookutils import *

def add_info(report, ui):

    # if it's a stacktrace, report it directly against the right component
    if "Stacktrace" in report:
        for external_component in ("/usr/lib/libnux", "/usr/lib/compiz/libunityshell", "/usr/lib/libunity"):
            for words in report["Stacktrace"].split():
                if words.startswith(external_component):
                    report.add_package_info(apport.packaging.get_file_package(words))
                    return
        report.add_hooks_info(ui, srcpackage='xorg')
        return

    
    # ok, add maximum interesting info without too much spam
    report['CompizPlugins'] = command_output(['gconftool-2',
        '--get', '/apps/compiz-1/general/screen0/options/active_plugins'])
    # User configuration
    report['GconfCompiz'] = command_output(['gconftool-2', '-R', '/apps/compiz-1'])


    # add all relevant info like xorg ones
    report.add_hooks_info(ui, srcpackage='xorg')
    
