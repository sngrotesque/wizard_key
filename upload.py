from git.repo import Repo
import subprocess
import sys
import re
import os

DEFINED_FOLDER_PATH = '.'
DEFINED_REMOTE_URL  = 'git@github.com:sngrotesque/wizard_key.git'
DEFINED_COMMIT      = 'The wizard\'s universal key'
DEFINED_VERSION     = 'v0.8'

def RunPopen(cmd :str):
    p = subprocess.Popen(cmd, shell = True, stdout = subprocess.PIPE)
    return p.stdout.read().decode()

def RunCommand(cmd :str):
    return subprocess.call(cmd, shell = True)

class git_process:
    def __init__(self, folder_path :str = DEFINED_FOLDER_PATH):
        self.repo              = Repo.init(folder_path)
        self.DefinedRemoteUrl  = DEFINED_REMOTE_URL
        self.DefinedCommit     = DEFINED_COMMIT
        self.DefinedVersion    = DEFINED_VERSION
        self.DefinedFolderPath = folder_path

        self.compiled_path     = '_compiled'
        self.path              = os.listdir(folder_path)

        if os.path.exists('.git'):
            self.path.remove('.git')
        if os.path.exists(self.compiled_path):
            self.path.remove('_compiled')
        # self.path.remove('upload.py')

    def view(self):
        print(f'>>>> +---------- 现有文件(BEGIN) ----------+')
        for fileName in self.path:
            print(f'>>>> + {fileName:<35} +')
        print(f'>>>> +---------- 现有文件(END)   ----------+')

    def versionCheck(self):
        branch_version        = RunPopen('git branch')
        
        branch_name_0_regular = r'\s+(v[\d.]+\-\w+)'
        branch_name_1_regular = r'\s+(v[\d.]+_\w+)'
        branch_name_2_regular = r'\s+(v[\d.]+)'
        branch_name_3_regular = r'\s+(\w+\_v[\d.]+)'
        
        versionList = \
            re.findall(branch_name_0_regular, branch_version, re.S) + \
            re.findall(branch_name_1_regular, branch_version, re.S) + \
            re.findall(branch_name_2_regular, branch_version, re.S) + \
            re.findall(branch_name_3_regular, branch_version, re.S)
        
        args = '-b' if self.DefinedVersion not in versionList else ''
        RunCommand(f'git checkout {args} {self.DefinedVersion}')

    def init(self):
        try:
            self.repo.create_remote(name='origin', url=self.DefinedRemoteUrl)
        except:
            pass

    def add(self, git_folder_path :str = None):
        if git_folder_path:
            git_folder_newPath = git_folder_path.replace('.git', 'temp_[git]')
            os.rename(git_folder_path, git_folder_newPath)
        
        for file_name in self.path:
            self.repo.index.add(items = file_name)
        
        if git_folder_path:
            self.repo.index.remove(git_folder_newPath, r=True)
            os.rename(git_folder_newPath, git_folder_path)

    def push(self):
        self.repo.index.commit(self.DefinedCommit)
        self.repo.remote().push(self.DefinedVersion)

try:
    if (sys.argv[1].lower() == 'yes') or (sys.argv[1].lower() == 'y'):
        os.system('git rm --cached -r *')
except IndexError:
    print('未指定是否从git暂存中删除所有暂存内容，默认为No。')

git = git_process()
git.init()
git.versionCheck()
git.view()
git.add()
git.push()
