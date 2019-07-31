import yaml
import os
import logging
import shutil
import re
import sys
import subprocess

sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..', 'project'))
from import_libs import ConfigParser as ConfigParser

logging.basicConfig(level=logging.DEBUG, format='%(levelname)s - %(name)s - %(message)s')

if __name__ == '__main__':
    parser = ConfigParser()
    config = parser.config
    libs = parser.libs
    
    for target, target_params in config.items():
        for lib, lib_info in libs[target].items():
            if lib in target_params['required_libs']:
                make_path = os.path.join(target, lib_info['target_dir'], 'armgcc')
                make_cmd = ['make', '-C', make_path]
                subprocess.run(make_cmd, check=True)
