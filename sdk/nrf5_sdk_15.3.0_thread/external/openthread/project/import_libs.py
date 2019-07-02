import yaml
import os
import logging
import shutil
import re
import sys

logging.basicConfig(level=logging.INFO, format='%(levelname)s - %(name)s - %(message)s')

class ConfigParser:

    PARAMS = ['required_libs', 'dropped_libs', 'compilers']

    def __init__(self, config_file_name='import_libs_config.yaml', libs_file_name='known_libs.yaml'):
        self.logger = logging.getLogger(type(self).__name__)
        self.libs = {}
        self.load_config(config_file_name)
        self.load_libs(libs_file_name)
        self.parse_libs()
        self.validate_config()

    def load_config(self, config_file_name='import_libs_config.yaml'):
        try:
            self.config_file_path = os.path.join(os.path.dirname(__file__), config_file_name)
            self.logger.debug('Loading config from file "{}"'.format(self.config_file_path))

            with open(self.config_file_path) as config_file:
                self.config = yaml.safe_load(config_file)
        except FileNotFoundError as ex:
            self.logger.error('"{}" not found'.format(config_file_name))
            self.config = {}
            raise ex
    
    def load_libs(self, libs_file_name='known_libs.yaml'):
        try:
            self.libs_file_path = os.path.join(os.path.dirname(__file__), libs_file_name)
            self.logger.debug('Loading libs from file "{}"'.format(self.libs_file_path))

            with open(self.libs_file_path) as libs_file:
                self.libs_raw = yaml.safe_load(libs_file)
        except FileNotFoundError as ex:
            self.logger.error('"{}" not found'.format(libs_file_name))
            self.libs_raw = {}
            raise ex

    def parse_libs(self):
        for target in self.config:
            self.libs[target] = {}
            for lib, lib_params in self.libs_raw.items():
                lib_renamed = lib.format(target_name=target)
                if lib_params:
                    self.libs[target][lib_renamed] = lib_params.copy()
                    if 'target_dir' in lib_params:
                        self.libs[target][lib_renamed]['target_dir'] = lib_params['target_dir'].format(target_name=target)
                    if 'src_path' in lib_params:
                        self.libs[target][lib_renamed]['src_path'] = lib_params['src_path'].format(target_name=target)
                else:
                    self.libs[target][lib_renamed] = {}

    def validate_config(self):
        for target, target_params in self.config.items():
            for required_param in self.PARAMS:
                if required_param not in target_params:
                    self.logger.warning('Missing param "{}" for "{}"'.format(required_param, target))
                    self.config[target][required_param] = []
            for param in target_params:
                if param not in self.PARAMS:
                    self.logger.error('Unexpected param "{}" for "{}"'.format(param, target))
                    raise AttributeError('Unexpected param "{}" for "{}"'.format(param, target))
            
            for lib in target_params['required_libs']:
                if lib not in self.libs[target]:
                    self.logger.error('Unexpected required lib "{}" for target "{}"'.format(lib, target))
                    raise AttributeError('Unexpected required lib "{}" for "{}"'.format(lib, target))

            for lib in target_params['dropped_libs']:
                if lib not in self.libs[target]:
                    self.logger.error('Unexpected dropped lib "{}" for target "{}"'.format(lib, target))
                    raise AttributeError('Unexpected dropped lib "{}" for "{}"'.format(lib, target))

            for lib in self.libs[target]:
                if lib not in target_params['required_libs'] and lib not in target_params['dropped_libs']:
                    self.logger.error('Lib "{}" not handled for target "{}"'.format(lib, target))
                    raise AttributeError('Lib "{}" not handled for "{}"'.format(lib, target))  

            bad_libs = [lib for lib in target_params['dropped_libs'] if lib in target_params['required_libs']]
            if len(bad_libs) is not 0:
                self.logger.error('Libs "{}" added as both requested and dropped for target "{}"'.format(' '.join(bad_libs), target))
                raise AttributeError('Libs "{}" added as both requested and dropped for "{}"'.format(' '.join(bad_libs), target))

class Compiler:
    LIB_LIKE_PATTERN = '^(.*)\.a$|^(.*)\.lib$'

    def __init__(self, libs):
        self.logger = logging.getLogger(type(self).__name__)
        self.libs = libs

    def get_from_path(self, target, lib, lib_dir=None):
        lib_dir = lib_dir or self.libs[lib]['target_dir']
        lib_dir = os.path.normcase(lib_dir)

        if target not in lib:
            lib = lib + '-{}'.format(target)

        path = self.PATH_FROM_TEMPLATE.format(lib_name=lib, lib_dir=lib_dir, target=target)
        self.logger.debug('Constructed "from" path "{}" for lib "{}"'.format(path, lib))
        return path
    
    def get_to_path(self, target, lib, lib_dir=None):
        lib_dir = lib_dir or self.libs[lib]['target_dir']
        lib_dir = os.path.normcase(lib_dir)
        path = self.PATH_TO_TEMPLATE.format(lib_name=lib, lib_dir=lib_dir, target=target)
        self.logger.debug('Constructed "to" path "{}" for lib "{}"'.format(path, lib))
        return path

class GCC(Compiler):
    PATH_FROM_TEMPLATE = os.path.join('{target}', '{lib_dir}', 'armgcc', '{lib_name}.a')
    PATH_TO_TEMPLATE = os.path.join('..', 'lib', '{target}', 'gcc', '{lib_name}.a')

class Keil(Compiler):
    PATH_FROM_TEMPLATE = os.path.join('{target}', '{lib_dir}', 'arm5_no_packs', '_build', '{lib_name}.lib')
    PATH_TO_TEMPLATE = os.path.join('..', 'lib', '{target}', 'keil', '{lib_name}.lib')

class IAR(Compiler):
    PATH_FROM_TEMPLATE = os.path.join('{target}', '{lib_dir}', 'iar', '_build', 'ot_{target}_{lib_name}.a')
    PATH_TO_TEMPLATE = os.path.join('..', 'lib', '{target}', 'iar', '{lib_name}.a')

    def get_from_path(self, target, lib):
        lib_dir = self.libs[lib]['target_dir']

        if target not in lib:
            lib = lib + '-{}'.format(target)

        lib = lib.replace('-', '_')
        return super().get_from_path(target, lib, lib_dir)

COMPILER_MAP = {'gcc': GCC, 'keil': Keil, 'iar': IAR}

if __name__ == '__main__':
    parser = ConfigParser()
    config = parser.config
    libs = parser.libs

    shutil.rmtree(os.path.join('..', 'lib'))

    if len(sys.argv) > 1:
        compilers = [compiler.casefold() for compiler in sys.argv[1:]]
    else:
        compilers = None

    all_libs = set([])
    for target, target_libs in libs.items():
        all_libs.update(target_libs)

    for _, _, filenames in os.walk('lib'):
        for file_name in filenames:
            match = re.match(Compiler.LIB_LIKE_PATTERN, file_name)
            if match:
                if match.groups()[0] not in all_libs:
                    logging.error("Unknown lib present {} ({})".format(match.groups()[0], file_name))
                    raise AttributeError("Unknown lib present {} ({})".format(match.groups()[0], file_name))

    for target, target_params in config.items():
        logging.debug('Processing target {}'.format(target))
        for compiler_name in target_params['compilers']:
            if compilers and compiler_name.casefold() not in compilers:
                continue
            compiler_class = COMPILER_MAP.get(compiler_name.casefold())
            compiler = compiler_class(libs[target])
            logging.debug('Compiler {}'.format(compiler_name))
            logging.debug('Compiler class {}'.format(compiler_class))

            os.makedirs(os.path.join('..', 'lib', '{target}', '{compiler}').format(target=target, compiler=compiler_name.casefold()))

            for lib in target_params['required_libs']:
                from_path = compiler.get_from_path(target, lib)
                to_path = compiler.get_to_path(target, lib)

                logging.info('Copying {} to {}'.format(from_path, to_path))
                shutil.copy(from_path, to_path)
