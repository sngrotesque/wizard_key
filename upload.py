from subprocess import Popen, PIPE, call
from git.repo import Repo
import re, os

def RunPopen(cmd :str):
    p = Popen(cmd, shell=True, stdout=PIPE)
    return p.stdout.read().decode()

def RunCommand(cmd :str):
    call(cmd, shell=True)

def deleteListData(OriginalList :list, dataContent: str):
    x = 0
    while x < len(OriginalList):
        if OriginalList[x][:3] == dataContent:
            OriginalList.pop(x);
            x -= 1
        x += 1

class git_process:
    def __init__(self, folder_path :str = '.'):
        self.repo              = Repo.init(folder_path)
        self.DefinedRemoteUrl  = 'git@github.com:sngrotesque/WMKC.git'
        self.DefinedCommit     = 'Wizard Magic Key Cyber'
        self.DefinedVersion    = 'v6.4.0-dev'
        self.DefinedFolderPath = folder_path

        self.path = os.listdir(folder_path)
        '''\
        此处的排序逻辑是隐藏目录优先级最高，其次是名称长的，最后是文件。
        如下：
            .1234/
            .git/
            12345/
            123/
            hello.c
            test.c
        '''
        self.path.remove('.git')
        self.path.remove('keys')
        self.path.remove('Reference')
        # self.path.remove('Documents')
        self.path.remove('misc')
        deleteListData(self.path, '___')

    def view(self):
        print(f'>>>> +---------- 现有文件(BEGIN) ----------+')
        for fileName in self.path:
            print(f'>>>> + {fileName:<35} +')
        print(f'>>>> +---------- 现有文件(END)   ----------+')

    def versionCheck(self):
        branch_version = RunPopen('git branch')
        
        branch_name_0_regular = r'\s+(v[\d.]+\-\w+)'
        branch_name_1_regular = r'\s+(v[\d.]+_\w+)'
        branch_name_2_regular = r'\s+(v[\d.]+)'
        branch_name_3_regular = r'\s+(\w+\_v[\d.]+)'
        
        versionList_0 = re.findall(branch_name_0_regular, branch_version, re.S)
        versionList_1 = re.findall(branch_name_1_regular, branch_version, re.S)
        versionList_2 = re.findall(branch_name_2_regular, branch_version, re.S)
        versionList_3 = re.findall(branch_name_3_regular, branch_version, re.S)
        
        versionList = versionList_0 + versionList_1 + versionList_2 + versionList_3
        
        if self.DefinedVersion not in versionList:
            RunCommand(f'git checkout -b {self.DefinedVersion}')
        if self.DefinedVersion in versionList:
            RunCommand(f'git checkout {self.DefinedVersion}')

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

git = git_process()
git.init()
git.versionCheck()
git.view()
git.add()
git.push()
