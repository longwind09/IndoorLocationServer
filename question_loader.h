/********************************************************************
	Author:		Wang Yu
	Created:	2013/07/10
	Revised:	2013/08/09   15:40
	File Base:	datafile_util

	Purpose:	题目加载类。
            提供数据文件的读取、修改、保存，并提供公共接口可以获取题目内容。
*********************************************************************/
#ifndef LOAD_DATA_QUESTION_LOADER_H_
#define LOAD_DATA_QUESTION_LOADER_H_

#include "question.h"

struct tm;
class TiXmlElement;

// Possible usage:
// QuestionLoader instance("myfile");
// instance.Load(questions, question_map);
class QuestionLoader
{
    public:
        QuestionLoader() {}
        QuestionLoader ( const std::string& file_name );
        QuestionLoader ( const char* file_name );
        ~QuestionLoader() {}

        inline const std::string& file_name() const
        {
            return file_name_;
        }
        inline void set_file_name ( const std::string& path )
        {
            file_name_ = path;
        }

        // 读取文件中所有的题目，存放在questions和question_map中
        // question_map是以题目id为key建立的map索引
        // 返回题目总数。
        int Load ( question::QuestionVec* question_list,
                   question::QuestionPtrMap* question_ptr_map );

        int Load ( question::QuestionPtrMap* question_ptr_map );

        question* CreateQuestionPtr ( TiXmlElement* ques_element );
        const question& CreateQuestion ( TiXmlElement* ques_element );

    private:
        // 将文件中字符串形式的日期转化为time_t
        time_t ParseTimeT ( const char* time_in_string );
        // 通过题目根元素节点解析出选项和答案
        void ParseOptionsAndAnswers ( const TiXmlElement& ques_element, //题目根元素节点
                                      std::vector<std::string>* options,
                                      std::vector<int>* answers );
        // 将表示全部图片名称的一整串字符串分隔，解析成多个名称
        void ParseImageNames ( const char* images, std::vector<std::string>* img_names );
        char* UTF8ToGBK ( const char* in );

        std::string file_name_;
};


class TextFileUtil
{
    public:
        bool parseFile ( std::string file, std::vector<std::string> &segs );
        static void split ( const std::string str, std::vector<std::string> &vec, char token );
};

#endif