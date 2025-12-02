#pragma once

#include <string>
#include <vector>
#include <map>
#include "course_mark.h"

//***************************************************************************************************************************************************
class ITstudent 
{
public:
    //***********************************************************************************************************************************************
    ITstudent();
    ITstudent(const std::string& _first_name, const std::string& _last_name, int _student_id, const std::string& _programme, const std::vector<course_mark>& _course_marks);

    // getters
    std::string get_full_name() const;
    std::string get_first_name() const;
    std::string get_last_name() const;
    int get_student_id() const;
    std::string get_programme() const;
    std::vector<course_mark> get_course_marks() const;

    // setters
    void set_full_name(const std::string& _first_name, const std::string& _last_name);
    void set_first_name(const std::string& _first_name);
    void set_last_name(const std::string& _last_name);
    void set_student_id(int _student_id);
    void set_programme(const std::string& _programme);
    void set_course_marks(const std::vector<course_mark>& _course_marks);

    // other functions
    float calculate_average() const;
    void print_student_info() const;

    // XML functions
    std::string to_XML() const;
    static ITstudent from_XML(const std::string& xml_content);

private:
    //***********************************************************************************************************************************************
    int student_id;
    std::string programme;
    std::string first_name, last_name;
    std::vector<course_mark> course_marks;
};