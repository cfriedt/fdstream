/*
 * MIT License
 *
 * Copyright (c) 2016 The LLVM Compiler Infrastructure
 * Copyright (c) 2016 Christopher Friedt
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef com_github_cfriedt_fstream_
#define com_github_cfriedt_fstream_

//#include <__config>
#include <ostream>
#include <istream>
//#include <__locale>
#include <cstdio>

#include <map>

//#include <__undef_min_max>

//_LIBCPP_BEGIN_NAMESPACE_STD

namespace com {
namespace github {
namespace cfriedt {


class filebuf
    : public std::streambuf
{
public:
//    typedef _CharT                           char_type;
//    typedef _Traits                          traits_type;
//    typedef typename traits_type::int_type   int_type;
//    typedef typename traits_type::pos_type   pos_type;
//    typedef typename traits_type::off_type   off_type;
//    typedef typename traits_type::state_type state_type;

    // 27.9.1.2 Constructors/destructor:
    filebuf();
    filebuf( int fd, std::ios_base::openmode mode );
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    filebuf(filebuf&& __rhs);
#endif
    virtual ~filebuf();

    // 27.9.1.3 Assign/swap:
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    filebuf& operator=(filebuf&& __rhs);
#endif
    void swap(filebuf& __rhs);

    // 27.9.1.4 Members:
    _LIBCPP_INLINE_VISIBILITY
    bool is_open() const;
#ifndef _LIBCPP_HAS_NO_GLOBAL_FILESYSTEM_NAMESPACE
    filebuf* open(const char* __s, std::ios_base::openmode __mode);
    _LIBCPP_INLINE_VISIBILITY
    filebuf* open(const std::string& __s, std::ios_base::openmode __mode);
#endif
    filebuf* close();

protected:
    // 27.9.1.5 Overridden virtual functions:
    virtual int_type underflow();
    virtual int_type pbackfail(int_type __c = traits_type::eof());
    virtual int_type overflow (int_type __c = traits_type::eof());
    virtual basic_streambuf* setbuf(char_type* __s, std::streamsize __n);
    virtual pos_type seekoff(off_type __off, std::ios_base::seekdir __way,
                             std::ios_base::openmode __wch = std::ios_base::in | std::ios_base::out);
    virtual pos_type seekpos(pos_type __sp,
                             std::ios_base::openmode __wch = std::ios_base::in | std::ios_base::out);
    virtual int sync();
    virtual void imbue(const std::locale& __loc);

//private:
    char*       __extbuf_;
    const char* __extbufnext_;
    const char* __extbufend_;
    char __extbuf_min_[8];
    size_t __ebs_;
    char_type* __intbuf_;
    size_t __ibs_;
    FILE* __file_;
    int   __file_fd_;
    bool __do_not_close_file_;
    const std::codecvt<char_type, char, mbstate_t>* __cv_;
    mbstate_t __st_;
    mbstate_t __st_last_;
    std::ios_base::openmode __om_;
    std::ios_base::openmode __cm_;
    bool __owns_eb_;
    bool __owns_ib_;
    bool __always_noconv_;

    bool __read_mode();
    void __write_mode();

    static const std::map<std::ios_base::openmode,const char *> mode_to_str;
    static const std::map<const char *,std::ios_base::openmode> str_to_mode;
};


// ifstream


class ifstream
    : public std::istream
{
public:
//    typedef _CharT                         char_type;
//    typedef _Traits                        traits_type;
//    typedef typename traits_type::int_type int_type;
//    typedef typename traits_type::pos_type pos_type;
//    typedef typename traits_type::off_type off_type;

    _LIBCPP_INLINE_VISIBILITY
    ifstream();
    _LIBCPP_INLINE_VISIBILITY
    ifstream( filebuf *buf );
#ifndef _LIBCPP_HAS_NO_GLOBAL_FILESYSTEM_NAMESPACE
    _LIBCPP_INLINE_VISIBILITY
    explicit ifstream(const char* __s, std::ios_base::openmode __mode = std::ios_base::in);
    _LIBCPP_INLINE_VISIBILITY
    explicit ifstream(const std::string& __s, std::ios_base::openmode __mode = std::ios_base::in);
#endif
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    ifstream(ifstream&& __rhs);
#endif

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    ifstream& operator=(ifstream&& __rhs);
#endif
    _LIBCPP_INLINE_VISIBILITY
    void swap(ifstream& __rhs);

    _LIBCPP_INLINE_VISIBILITY
    filebuf* rdbuf() const;
    _LIBCPP_INLINE_VISIBILITY
    bool is_open() const;
#ifndef _LIBCPP_HAS_NO_GLOBAL_FILESYSTEM_NAMESPACE
    void open(const char* __s, std::ios_base::openmode __mode = std::ios_base::in);
    void open(const std::string& __s, std::ios_base::openmode __mode = std::ios_base::in);
#endif
    _LIBCPP_INLINE_VISIBILITY
    void close();

//private:
protected:
    filebuf __sb_;
};

// ofstream


class ofstream
    : public std::ostream
{
public:
//    typedef _CharT                         char_type;
//    typedef _Traits                        traits_type;
//    typedef typename traits_type::int_type int_type;
//    typedef typename traits_type::pos_type pos_type;
//    typedef typename traits_type::off_type off_type;

    _LIBCPP_INLINE_VISIBILITY
    ofstream();
    _LIBCPP_INLINE_VISIBILITY
    ofstream( filebuf *buf );
    _LIBCPP_INLINE_VISIBILITY
    explicit ofstream(const char* __s, std::ios_base::openmode __mode = std::ios_base::out);
    _LIBCPP_INLINE_VISIBILITY
    explicit ofstream(const std::string& __s, std::ios_base::openmode __mode = std::ios_base::out);
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    ofstream(ofstream&& __rhs);
#endif

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    ofstream& operator=(ofstream&& __rhs);
#endif
    _LIBCPP_INLINE_VISIBILITY
    void swap(ofstream& __rhs);

    _LIBCPP_INLINE_VISIBILITY
    filebuf* rdbuf() const;
    _LIBCPP_INLINE_VISIBILITY
    filebuf* rdbuf( filebuf* __sb );
    _LIBCPP_INLINE_VISIBILITY
    bool is_open() const;
#ifndef _LIBCPP_HAS_NO_GLOBAL_FILESYSTEM_NAMESPACE
    void open(const char* __s, std::ios_base::openmode __mode = std::ios_base::out);
    void open(const std::string& __s, std::ios_base::openmode __mode = std::ios_base::out);
#endif
    _LIBCPP_INLINE_VISIBILITY
    void close();

//private:
protected:
    filebuf __sb_;
};


// fstream


class fstream
    : public std::iostream
{
public:
//    typedef _CharT                         char_type;
//    typedef _Traits                        traits_type;
//    typedef typename traits_type::int_type int_type;
//    typedef typename traits_type::pos_type pos_type;
//    typedef typename traits_type::off_type off_type;

    _LIBCPP_INLINE_VISIBILITY
    fstream();
    _LIBCPP_INLINE_VISIBILITY
    fstream( filebuf *buf );
#ifndef _LIBCPP_HAS_NO_GLOBAL_FILESYSTEM_NAMESPACE
    _LIBCPP_INLINE_VISIBILITY
    explicit fstream(const char* __s, std::ios_base::openmode __mode = std::ios_base::in | std::ios_base::out);
    _LIBCPP_INLINE_VISIBILITY
    explicit fstream(const std::string& __s, std::ios_base::openmode __mode = std::ios_base::in | std::ios_base::out);
#endif
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    fstream(fstream&& __rhs);
#endif

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    fstream& operator=(fstream&& __rhs);
#endif
    _LIBCPP_INLINE_VISIBILITY
    void swap(fstream& __rhs);

    _LIBCPP_INLINE_VISIBILITY
    filebuf* rdbuf() const;
    _LIBCPP_INLINE_VISIBILITY
    bool is_open() const;
#ifndef _LIBCPP_HAS_NO_GLOBAL_FILESYSTEM_NAMESPACE
    void open(const char* __s, std::ios_base::openmode __mode = std::ios_base::in | std::ios_base::out);
    void open(const std::string& __s, std::ios_base::openmode __mode = std::ios_base::in | std::ios_base::out);
#endif
    _LIBCPP_INLINE_VISIBILITY
    void close();

//private:
protected:
    filebuf __sb_;
};

//_LIBCPP_END_NAMESPACE_STD

}
}
} // com.github.cfriedt

#endif  // com_github_cfriedt_fstream_
