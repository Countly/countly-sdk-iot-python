import platform
import socket
import os
import locale


class system_info:
    @staticmethod
    def machine():
        return platform.machine()
    @staticmethod
    def os_uname():
     return os.uname()
    @staticmethod
    def os_version():
        return platform.version()
    @staticmethod
    def platform():
        return platform.platform()
    @staticmethod
    def system():
        return platform.system()
    @staticmethod
    def ip():
        return socket.gethostbyname(socket.gethostname())
    @staticmethod
    def locale():
        return locale.getdefaultlocale()
