// to change blade test work path

#ifndef __BLADE_TEST_DIR_H
#define __BLADE_TEST_DIR_H

#include <unistd.h>
#include <linux/limits.h>
#include <string>
#include <iostream>
#include "gtest/gtest.h"

#ifndef __FULLPATH__
#error __FULLPATH__ not defined, add '-D__FULLPATH__=`readlink -f $SOURCE`' at extra_cppflags in BUILD file
#else
#define __GTEST_STR_IMPL__(x)   #x
#define __GTEST_STR__(x)        __GTEST_STR_IMPL__(x)
#define BLADE_TEST_PWD(Name)    static BladeTestEvn __##Name(__GTEST_STR__(__FULLPATH__))
#define BLADE_TEST_SET_PWD      BladeTestEvn::setWorkPath(__GTEST_STR__(__FULLPATH__))
#define BLADE_TEST_REVERT_PWD   BladeTestEvn::revertWorkPath()

#define BLADE_TEST_COMMON_ENV   static ::testing::Environment* const __blade_test_common_env__ = ::testing::AddGlobalTestEnvironment(new CommonPrepare)
#endif

class BladeTestEvn
{
public:
    static void setWorkPath(const char *workpath) {
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        Inst().oldpwd = dirName(cwd);
        std::string newpwd = dirName(workpath);
        chdir(newpwd.c_str());
        std::cout<<"change to "<<newpwd<<std::endl;
    }
    static void revertWorkPath() {
        chdir(Inst().oldpwd.c_str());
        std::cout<<"revert to "<<Inst().oldpwd<<std::endl;
    }
private:
    static std::string dirName(const char*fullpath) {
        std::string str(fullpath);
        size_t found = str.find_last_of("/\\");
        return str.substr(0,found);
    }
    static BladeTestEvn& Inst() {
        static BladeTestEvn _inst;
        return _inst;
    }
    std::string oldpwd;
};

class CommonPrepare:public ::testing::Environment {
public:
    CommonPrepare() {}
    virtual ~CommonPrepare() {}
    virtual void SetUp(){BLADE_TEST_SET_PWD;}
    virtual void TearDown(){BLADE_TEST_REVERT_PWD;}
};

#endif

