/********************************************************************
	Author:		Wang Yu
	Created:	2013/07/10
	Revised:	2013/08/09   19:23
	File Base:	datafile_util

	Purpose:	QuestionLoader的实现。
*********************************************************************/

#include "stdafx.h"
#include "question_loader.h"
#include <ctime>
#include <windows.h>


QuestionLoader::QuestionLoader ( const std::string& file_name )
{
    set_file_name ( file_name );
}

QuestionLoader::QuestionLoader ( const char* file_name )
{
    set_file_name ( file_name );
}

int QuestionLoader::Load ( question::QuestionVec* question_list,
                           question::QuestionPtrMap* question_ptr_map )
{
    TiXmlDocument doc ( file_name_.c_str() );

    if ( doc.LoadFile ( TIXML_ENCODING_UTF8 ) == NULL )
    {
        return 0;
    }

    TiXmlElement* root = doc.RootElement();
    TiXmlElement* ques_element = root->FirstChildElement ( "question" );
    int ques_count = 0;

    while ( ques_element != NULL )
    {
        question* ques = CreateQuestionPtr ( ques_element );
        question_list->push_back ( *ques );
        ques_element = ques_element->NextSiblingElement();
        ++ques_count;
        delete ques;
    }

    for ( int i = 0; i < question_list->size(); i++ )
    {
        question_ptr_map->insert ( make_pair ( ( *question_list ) [i].qid(),
                                               & ( *question_list ) [i] ) );
    }

    // 不需要手动释放，TiXmlDocument析构时会自动delete
    // 如果是用指针doc = new TiXmlDocument(...)，这时需要手动delete
    //delete root;
    //delete ques_element;
    return ques_count;
}

int QuestionLoader::Load ( question::QuestionPtrMap* question_ptr_map )
{
    TiXmlDocument doc ( file_name_.c_str() );

    if ( doc.LoadFile ( TIXML_ENCODING_UTF8 ) == NULL )
    {
        return 0;
    }

    TiXmlElement* root = doc.RootElement();
    TiXmlElement* ques_element = root->FirstChildElement ( "question" );
    int ques_count = 0;

    while ( ques_element != NULL )
    {
        question* ques = CreateQuestionPtr ( ques_element );
        question_ptr_map->insert ( make_pair ( ques->qid(), ques ) );
        ques_element = ques_element->NextSiblingElement();
        ++ques_count;
        //delete ques;
    }

    return ques_count;
}

question* QuestionLoader::CreateQuestionPtr ( TiXmlElement* ques_element )
{
    question* ques = new question();
    ques->set_qid ( ques_element->FirstChildElement ( "id" )->GetText() );
    char* category = UTF8ToGBK (
                         ques_element->FirstChildElement ( "category" )->GetText() );
    ques->set_category ( category );
    char* role = UTF8ToGBK (
                     ques_element->FirstChildElement ( "role" )->GetText() );
    ques->set_role ( role );
    char* difficulty = UTF8ToGBK (
                           ques_element->FirstChildElement ( "difficulty" )->GetText() );
    ques->set_difficulty ( difficulty );
    int full_score;
    sscanf_s ( ques_element->FirstChildElement ( "score" )->GetText(), "%d", &full_score );
    ques->set_full_score ( full_score );
    char* stem = UTF8ToGBK (
                     ques_element->FirstChildElement ( "body" )->GetText() );
    ques->set_stem ( stem );
    std::vector<std::string>* options = new std::vector<std::string>;
    std::vector<int>* answers = new std::vector<int>;
    ParseOptionsAndAnswers ( *ques_element, options, answers );
    ques->set_options ( *options );
    ques->set_answers ( *answers );
    ques->set_entry_time ( ParseTimeT ( ques_element->FirstChildElement ( "entryTime" )->GetText() ) );
    std::vector<std::string>* img_names = new std::vector<std::string>;
    ParseImageNames ( ques_element->FirstChildElement ( "picPath" )->GetText(),
                      img_names );
    ques->set_img_names ( *img_names );
    delete options;
    delete answers;
    delete img_names;
    return ques;
}

const question& QuestionLoader::CreateQuestion ( TiXmlElement* ques_element )
{
    return *CreateQuestionPtr ( ques_element );
}

time_t QuestionLoader::ParseTimeT ( const char* time_in_string )
{
    time_t entry_time;
    sscanf_s ( time_in_string, "%lld", &entry_time );
    return entry_time;
}

void QuestionLoader::ParseOptionsAndAnswers ( const TiXmlElement& ques_element,
        std::vector<std::string>* options,
        std::vector<int>* answers )
{
    const TiXmlElement* option = ques_element.FirstChildElement ( "option" );
    int index = 0;

    while ( option != NULL )
    {
        options->push_back ( UTF8ToGBK ( option->GetText() ) );

        if ( option->Attribute ( "answer" ) != NULL )
        {
            answers->push_back ( index );
        }

        option = option->NextSiblingElement ( "option" );
        ++index;
    }
}

void QuestionLoader::ParseImageNames ( const char* images,
                                       std::vector<std::string>* img_names )
{
    while ( images != NULL && *images != '\0' )
    {
        const char* start = images;

        while ( *images != ';' )
        {
            images++;
        }

        size_t length = ( images - start + 1 ) / sizeof ( char );
        char* file_name = new char[length];
        std::copy ( start, images, file_name );
        file_name[length - 1] = '\0';
        img_names->push_back ( UTF8ToGBK ( file_name ) );
        //copy(start, images, img_names->rbegin());
        ++images;
    }
}

char* QuestionLoader::UTF8ToGBK ( const char* in )
{
    //只是为了获得长度，没有转化
    int len = MultiByteToWideChar ( CP_UTF8, 0, in, -1, NULL, 0 );
    unsigned short* wszGBK = new unsigned short[len + 1];
    memset ( wszGBK, 0, len * 2 + 2 );
    MultiByteToWideChar ( CP_UTF8, 0, ( LPCSTR ) in, -1, ( LPWSTR ) wszGBK, len );
    len = WideCharToMultiByte ( CP_ACP, 0, ( LPWSTR ) wszGBK, -1, NULL, 0, NULL, NULL );
    char* szGBK = new char[len + 1];
    memset ( szGBK, '\0', len + 1 );
    WideCharToMultiByte ( CP_ACP, 0, ( LPWSTR ) wszGBK, -1, szGBK, len, NULL, NULL );
    //将szGBK的内容赋给szOut ，这样即使szGBK被释放也能得到正确的值
    //memset(out,'\0',strlen(szGBK)+1);
    //memcpy(out,szGBK,strlen(szGBK));
    //delete []szGBK;
    delete []wszGBK;
    return szGBK;
}

