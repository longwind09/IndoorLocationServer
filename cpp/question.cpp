#include "stdafx.h"

#include "question.h"


question::question() {}

//????
question::question ( const question& ques )
{
    new ( this ) question (
        ques.qid_,
        ques.stem_,
        ques.options_,
        ques.answers_,
        ques.img_names_,
        ques.full_score_,
        ques.entry_time_
    );
}
question::question ( const std::string& id,
                     const std::string& stem,
                     const std::vector<std::string>& options,
                     const std::vector<int>& answers,
                     const std::vector<std::string>& img_names,
                     const double& full_score,
                     const boost::posix_time::ptime& entry_time )
    : qid_ ( id ), stem_ ( stem ), full_score_ ( full_score ), entry_time_ ( entry_time )
{
    options_.assign ( options.begin(), options.end() );
    answers_.assign ( answers.begin(), answers.end() );
    img_names_.assign ( img_names.begin(), img_names.end() );
}

void question::set_stem ( const std::string& stem )
{
    stem_ = stem;
}

const std::string& question::stem() const
{
    return stem_;
}

void question::set_options ( const std::vector<std::string>& options )
{
    options_.assign ( options.begin(), options.end() );
}

const std::vector<std::string>& question::options() const
{
    return options_;
}

void question::set_answers ( const std::vector<int>& answers )
{
    answers_.assign ( answers.begin(), answers.end() );
}

const std::vector<int>& question::answers() const
{
    return answers_;
}

void question::set_img_names ( const std::vector<std::string>& img_names )
{
    img_names_.assign ( img_names.begin(), img_names.end() );
}

const std::vector<std::string>& question::img_names() const
{
    return img_names_;
}

void question::set_full_score ( const double full_score )
{
    full_score_ = full_score;
}

const double question::full_score() const
{
    return full_score_;
}

void question::set_category ( const std::string& categoty )
{
    category_ = categoty;
}
const std::string& question::category() const
{
    return category_;
}

void question::set_role ( const std::string& role )
{
    role_ = role;
}
const std::string& question::role() const
{
    return role_;
}

void question::set_difficulty ( const std::string& difficulty )
{
    difficulty_ = difficulty;
}
const std::string& question::difficulty() const
{
    return difficulty_;
}

void question::set_entry_time ( const time_t& entry_time )
{
    entry_time_ = boost::posix_time::from_time_t ( entry_time );
}

const boost::posix_time::ptime& question::entry_time() const
{
    return entry_time_;
}