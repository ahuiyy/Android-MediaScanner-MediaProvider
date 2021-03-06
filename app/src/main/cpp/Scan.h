//
// Created by liujieqin on 2020/6/19.
//



#ifndef SCAN_H
#define SCAN_H

#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <queue>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string>
#include "sqlite3.h"
#include <unistd.h>
#include <list>
#include <thread>
#include <pthread.h>
#include <mutex>

//#include <mediametadataretriever.h>
//#include <CharacterEncodingDetector.h>
//#include <media/stagefright/MediaSource.h>
//#include "../../../../../../AppData/Local/Android/Sdk/ndk-bundle/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/c++/v1/string"

namespace android {

#define MEDIA_NEED_UPDATE 2
#define MEDIA_NEED_INSERT 1
#define MEDIA_NO_UPDATE 0

    class Scan
    {
    private:
        enum mediaType
        {
            audio,
            video,
            folder
        };
        sqlite3 *mdb;
        bool firstScan;
        static int aa;
//        int insertcount;
//        char pathRoot[1024];
        int rootPathLen;
        std::list<std::string> mediaList;
        std::list<std::string> mediaListBuf;
//        char oldVolume[64];
        bool isNewVolume;
    public:
        Scan();
        ~Scan();
        int getId(const char* path);
        int ProcessDirectory(const char *path, int isNewVolume, bool isfirstScan);
        static int callback(void *data, int args_num, char **columnValue, char **columnName);
        sqlite3* creat_database();
    private:
        static void* flushToDB(void *ptr);
        bool insertFolder(sqlite3 *db, const char* path, int parentId);
        bool flush();
        void prescan();

        bool open_database(sqlite3* &mdb);
        bool scanFile(sqlite3 *db,const  char* path, mediaType type, int parentId, const int dirLayer);
        int checkFileNeedUpdate(const char* path, int mtime, mediaType type);
        sqlite3_stmt* queryData(const char* table, const char* projection[], int projectionSize, const char* selection, const char* index, const char* selectArg);
        bool updateFolderHaveMedia(sqlite3 *db, int id, mediaType type);
        bool delete_old_data(std::string& list);



        class sDirEntry {
        public:
            char    *abs_file_name_p;
            int     filelen;
            int     depth;

        public:
            sDirEntry(int dp, const char* fileName)
                    : depth(dp)
            {
                // printf("constructor sDirEntry\n");
                size_t len = strlen(fileName);
                abs_file_name_p = (char*)malloc(len + 1);
                if (NULL == abs_file_name_p) {
                    free(abs_file_name_p);
                }
                memcpy(abs_file_name_p, fileName, len);
                abs_file_name_p[len] = '\0';
                filelen = len;
            }



            ~sDirEntry()
            {
                // printf("destructor sDirEntry\n");
                depth = 0;
                filelen = 0;
                free(abs_file_name_p);
            }

            sDirEntry(const sDirEntry& r) {
                // printf("copy constructor sDirEntry\n");

                depth = r.depth;
                filelen = r.filelen;
                abs_file_name_p = (char*)malloc(filelen + 1);
                memcpy(abs_file_name_p, r.abs_file_name_p, filelen + 1);
            }
            //sDirEntry&
            void operator=(const sDirEntry& r) {
                // printf("copy constructor sDirEntry\n");

                depth = r.depth;
                filelen = r.filelen;
                abs_file_name_p = (char*)malloc(filelen + 1);
                memcpy(abs_file_name_p, r.abs_file_name_p, filelen + 1);
            }


        };

    };

};

#endif


