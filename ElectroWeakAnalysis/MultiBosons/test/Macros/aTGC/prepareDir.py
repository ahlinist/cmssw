import os

class makeDir:
    def __call__(self,dir):
        if not os.path.exists(dir): os.mkdir(dir)


class cleanDir:
    def __call__(self,dir):
        os.commands("rm %s/*"%dir)
