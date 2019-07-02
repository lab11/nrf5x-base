import shutil
import os
import sys
import logging

sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..', 'project'))
from import_libs import ConfigParser as ConfigParser

logging.basicConfig(level=logging.DEBUG, format='%(levelname)s - %(name)s - %(message)s')

if __name__ == '__main__':
    parser = ConfigParser()
    config = parser.config

    DIR = '.'

    for target in config:
        dir_name = os.path.join(DIR, target)
        subdirs = [os.path.join(dir_name, o) for o in os.listdir(dir_name) 
                    if os.path.isdir(os.path.join(dir_name,o))]
        for subdir in subdirs:
            logging.info('Removing {}'.format(subdir))
            shutil.rmtree(subdir)
