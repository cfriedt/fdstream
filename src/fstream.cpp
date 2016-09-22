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

#include "cfriedt/fstream"

using namespace ::com::github::cfriedt;

const std::map<std::ios_base::openmode,std::string> filebuf::mode_to_str {
	{ std::ios_base::out, "w" },
	{ std::ios_base::out | std::ios_base::trunc, "w" },
	{ std::ios_base::out | std::ios_base::app, "a" },
	{ std::ios_base::app, "a" },
	{ std::ios_base::in, "r" },
	{ std::ios_base::in | std::ios_base::out, "r+" },
	{ std::ios_base::in | std::ios_base::out | std::ios_base::trunc, "w+" },
	{ std::ios_base::in | std::ios_base::out | std::ios_base::app, "a+" },
	{ std::ios_base::in | std::ios_base::app, "a+" },
	{ std::ios_base::out | std::ios_base::binary, "wb" },
	{ std::ios_base::out | std::ios_base::trunc | std::ios_base::binary, "wb" },
	{ std::ios_base::out | std::ios_base::app | std::ios_base::binary, "ab" },
	{ std::ios_base::app | std::ios_base::binary, "ab" },
	{ std::ios_base::in | std::ios_base::binary, "rb" },
	{ std::ios_base::in | std::ios_base::out | std::ios_base::binary, "r+b" },
	{ std::ios_base::in | std::ios_base::out | std::ios_base::trunc | std::ios_base::binary, "w+b" },
	{ std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary, "a+b" },
	{ std::ios_base::in | std::ios_base::app | std::ios_base::binary, "a+b" },
};

const std::map<std::string,std::ios_base::openmode> filebuf::str_to_mode {
	{ "w", std::ios_base::out | std::ios_base::trunc },
	{ "a", std::ios_base::out | std::ios_base::app },
	{ "r", std::ios_base::in },
	{ "r+", std::ios_base::in | std::ios_base::out },
	{ "w+", std::ios_base::in | std::ios_base::out | std::ios_base::trunc },
	{ "a+", std::ios_base::in | std::ios_base::out | std::ios_base::app },
	{ "wb", std::ios_base::out | std::ios_base::trunc | std::ios_base::binary },
	{ "ab", std::ios_base::out | std::ios_base::app | std::ios_base::binary },
	{ "rb", std::ios_base::in | std::ios_base::binary },
	{ "r+b", std::ios_base::in | std::ios_base::out | std::ios_base::binary },
	{ "w+b", std::ios_base::in | std::ios_base::out | std::ios_base::trunc | std::ios_base::binary },
	{ "a+b", std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary },
};

filebuf::filebuf(filebuf&& __rhs)
    : std::streambuf(__rhs)
{
    if (__rhs.__extbuf_ == __rhs.__extbuf_min_)
    {
        __extbuf_ = __extbuf_min_;
        __extbufnext_ = __extbuf_ + (__rhs.__extbufnext_ - __rhs.__extbuf_);
        __extbufend_ = __extbuf_ + (__rhs.__extbufend_ - __rhs.__extbuf_);
    }
    else
    {
        __extbuf_ = __rhs.__extbuf_;
        __extbufnext_ = __rhs.__extbufnext_;
        __extbufend_ = __rhs.__extbufend_;
    }
    __ebs_ = __rhs.__ebs_;
    __intbuf_ = __rhs.__intbuf_;
    __ibs_ = __rhs.__ibs_;
    __file_ = __rhs.__file_;
    __file_fd_ = __rhs.__file_fd_;
    __cv_ = __rhs.__cv_;
    __st_ = __rhs.__st_;
    __st_last_ = __rhs.__st_last_;
    __om_ = __rhs.__om_;
    __cm_ = __rhs.__cm_;
    __owns_eb_ = __rhs.__owns_eb_;
    __owns_ib_ = __rhs.__owns_ib_;
    __always_noconv_ = __rhs.__always_noconv_;
    if (__rhs.pbase())
    {
        if (__rhs.pbase() == __rhs.__intbuf_)
            this->setp(__intbuf_, __intbuf_ + (__rhs. epptr() - __rhs.pbase()));
        else
            this->setp((char_type*)__extbuf_,
                       (char_type*)__extbuf_ + (__rhs. epptr() - __rhs.pbase()));
        this->pbump(__rhs. pptr() - __rhs.pbase());
    }
    else if (__rhs.eback())
    {
        if (__rhs.eback() == __rhs.__intbuf_)
            this->setg(__intbuf_, __intbuf_ + (__rhs.gptr() - __rhs.eback()),
                                  __intbuf_ + (__rhs.egptr() - __rhs.eback()));
        else
            this->setg((char_type*)__extbuf_,
                       (char_type*)__extbuf_ + (__rhs.gptr() - __rhs.eback()),
                       (char_type*)__extbuf_ + (__rhs.egptr() - __rhs.eback()));
    }
    __rhs.__extbuf_ = 0;
    __rhs.__extbufnext_ = 0;
    __rhs.__extbufend_ = 0;
    __rhs.__ebs_ = 0;
    __rhs.__intbuf_ = 0;
    __rhs.__ibs_ = 0;
    __rhs.__file_ = 0;
    __rhs.__file_fd_ = -1;
    __rhs.__st_ = mbstate_t();
    __rhs.__st_last_ = mbstate_t();
    __rhs.__om_ = 0;
    __rhs.__cm_ = 0;
    __rhs.__owns_eb_ = false;
    __rhs.__owns_ib_ = false;
    __rhs.setg(0, 0, 0);
    __rhs.setp(0, 0);
}

filebuf::filebuf()
    : __extbuf_(0),
      __extbufnext_(0),
      __extbufend_(0),
      __ebs_(0),
      __intbuf_(0),
      __ibs_(0),
      __file_(0),
	  __file_fd_(-1),
      __cv_(nullptr),
      __st_(),
      __st_last_(),
      __om_(0),
      __cm_(0),
      __owns_eb_(false),
      __owns_ib_(false),
      __always_noconv_(false)
{
    if (std::has_facet<std::codecvt<char_type, char, mbstate_t> >(this->getloc()))
    {
        __cv_ = &std::use_facet<std::codecvt<char_type, char, mbstate_t> >(this->getloc());
        __always_noconv_ = __cv_->always_noconv();
    }
    setbuf(0, 4096);
}

filebuf::filebuf( int fd, std::ios_base::openmode __mode )
    : __extbuf_(0),
      __extbufnext_(0),
      __extbufend_(0),
      __ebs_(0),
      __intbuf_(0),
      __ibs_(0),
      __file_(0),
	  __file_fd_(-1),
      __cv_(nullptr),
      __st_(),
      __st_last_(),
      __om_(0),
      __cm_(0),
      __owns_eb_(false),
      __owns_ib_(false),
      __always_noconv_(false)
{
	std::map<std::ios_base::openmode,std::string>::const_iterator it;

	it = mode_to_str.find( __mode & ~std::ios_base::ate );

	const char *__strmod = mode_to_str.end() == it ? 0 : it->second.c_str();
	if ( 0 != __strmod ) {
		__file_ = fdopen( fd, __strmod );
		if ( 0 != __file_ ) {
			__file_fd_ = fd;
            __om_ = __mode;
            if (__mode & std::ios_base::ate)
            {
                if (fseek(__file_, 0, SEEK_END))
                {
                    fclose(__file_);
                    __file_ = 0;
                    __file_fd_ = -1;
                }
            }

		}
	}
}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

filebuf&
filebuf::operator=(filebuf&& __rhs)
{
    close();
    swap(__rhs);
    return *this;
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

filebuf::~filebuf()
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        close();
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
    if (__owns_eb_)
        delete [] __extbuf_;
    if (__owns_ib_)
        delete [] __intbuf_;
}


void
filebuf::swap(filebuf& __rhs)
{
    basic_streambuf::swap(__rhs);
    if (__extbuf_ != __extbuf_min_ && __rhs.__extbuf_ != __rhs.__extbuf_min_)
    {
        _VSTD::swap(__extbuf_, __rhs.__extbuf_);
        _VSTD::swap(__extbufnext_, __rhs.__extbufnext_);
        _VSTD::swap(__extbufend_, __rhs.__extbufend_);
    }
    else
    {
        ptrdiff_t __ln = __extbufnext_ - __extbuf_;
        ptrdiff_t __le = __extbufend_ - __extbuf_;
        ptrdiff_t __rn = __rhs.__extbufnext_ - __rhs.__extbuf_;
        ptrdiff_t __re = __rhs.__extbufend_ - __rhs.__extbuf_;
        if (__extbuf_ == __extbuf_min_ && __rhs.__extbuf_ != __rhs.__extbuf_min_)
        {
            __extbuf_ = __rhs.__extbuf_;
            __rhs.__extbuf_ = __rhs.__extbuf_min_;
        }
        else if (__extbuf_ != __extbuf_min_ && __rhs.__extbuf_ == __rhs.__extbuf_min_)
        {
            __rhs.__extbuf_ = __extbuf_;
            __extbuf_ = __extbuf_min_;
        }
        __extbufnext_ = __extbuf_ + __rn;
        __extbufend_ = __extbuf_ + __re;
        __rhs.__extbufnext_ = __rhs.__extbuf_ + __ln;
        __rhs.__extbufend_ = __rhs.__extbuf_ + __le;
    }
    _VSTD::swap(__ebs_, __rhs.__ebs_);
    _VSTD::swap(__intbuf_, __rhs.__intbuf_);
    _VSTD::swap(__ibs_, __rhs.__ibs_);
    _VSTD::swap(__file_, __rhs.__file_);
    _VSTD::swap(__file_fd_, __rhs.__file_fd_);
    _VSTD::swap(__cv_, __rhs.__cv_);
    _VSTD::swap(__st_, __rhs.__st_);
    _VSTD::swap(__st_last_, __rhs.__st_last_);
    _VSTD::swap(__om_, __rhs.__om_);
    _VSTD::swap(__cm_, __rhs.__cm_);
    _VSTD::swap(__owns_eb_, __rhs.__owns_eb_);
    _VSTD::swap(__owns_ib_, __rhs.__owns_ib_);
    _VSTD::swap(__always_noconv_, __rhs.__always_noconv_);
    if (this->eback() == (char_type*)__rhs.__extbuf_min_)
    {
        ptrdiff_t __n = this->gptr() - this->eback();
        ptrdiff_t __e = this->egptr() - this->eback();
        this->setg((char_type*)__extbuf_min_,
                   (char_type*)__extbuf_min_ + __n,
                   (char_type*)__extbuf_min_ + __e);
    }
    else if (this->pbase() == (char_type*)__rhs.__extbuf_min_)
    {
        ptrdiff_t __n = this->pptr() - this->pbase();
        ptrdiff_t __e = this->epptr() - this->pbase();
        this->setp((char_type*)__extbuf_min_,
                   (char_type*)__extbuf_min_ + __e);
        this->pbump(__n);
    }
    if (__rhs.eback() == (char_type*)__extbuf_min_)
    {
        ptrdiff_t __n = __rhs.gptr() - __rhs.eback();
        ptrdiff_t __e = __rhs.egptr() - __rhs.eback();
        __rhs.setg((char_type*)__rhs.__extbuf_min_,
                   (char_type*)__rhs.__extbuf_min_ + __n,
                   (char_type*)__rhs.__extbuf_min_ + __e);
    }
    else if (__rhs.pbase() == (char_type*)__extbuf_min_)
    {
        ptrdiff_t __n = __rhs.pptr() - __rhs.pbase();
        ptrdiff_t __e = __rhs.epptr() - __rhs.pbase();
        __rhs.setp((char_type*)__rhs.__extbuf_min_,
                   (char_type*)__rhs.__extbuf_min_ + __e);
        __rhs.pbump(__n);
    }
}


inline _LIBCPP_INLINE_VISIBILITY
void
swap(filebuf& __x, filebuf& __y)
{
    __x.swap(__y);
}


bool
filebuf::is_open() const
{
    return __file_ != 0;
}

#ifndef _LIBCPP_HAS_NO_GLOBAL_FILESYSTEM_NAMESPACE

filebuf*
filebuf::open(const char* __s, std::ios_base::openmode __mode)
{
    filebuf* __rt = 0;
    if (__file_ == 0)
    {
        __rt = this;
        std::map<std::ios_base::openmode,std::string>::const_iterator __it;
        __it = mode_to_str.find( __mode & ~std::ios_base::ate );
        const char* __mdstr = mode_to_str.end() == __it ? 0 : __it->second.c_str();
        __rt = 0 == __mdstr ? 0 : this;
        if (__rt)
        {
            __file_ = fopen(__s, __mdstr);
            if (__file_)
            {
                __file_fd_ = fileno( __file_ );
                __om_ = __mode;
                if (__mode & std::ios_base::ate)
                {
                    if (fseek(__file_, 0, SEEK_END))
                    {
                        fclose(__file_);
                        __file_ = 0;
                        __file_fd_ = -1;
                        __rt = 0;
                    }
                }
            }
            else
            {
                __rt = 0;
            }
        }
    }
    return __rt;
}

filebuf*
filebuf::open(const std::string& __s, std::ios_base::openmode __mode)
{
    return open(__s.c_str(), __mode);
}
#endif

std::streambuf*
filebuf::setbuf(char_type* __s, std::streamsize __n)
{
    this->setg(0, 0, 0);
    this->setp(0, 0);
    if (__owns_eb_)
        delete [] __extbuf_;
    if (__owns_ib_)
        delete [] __intbuf_;
    __ebs_ = __n;
    if (__ebs_ > sizeof(__extbuf_min_))
    {
        if (__always_noconv_ && __s)
        {
            __extbuf_ = (char*)__s;
            __owns_eb_ = false;
        }
        else
        {
            __extbuf_ = new char[__ebs_];
            __owns_eb_ = true;
        }
    }
    else
    {
        __extbuf_ = __extbuf_min_;
        __ebs_ = sizeof(__extbuf_min_);
        __owns_eb_ = false;
    }
    if (!__always_noconv_)
    {
        __ibs_ = std::max<std::streamsize>(__n, sizeof(__extbuf_min_));
        if (__s && __ibs_ >= sizeof(__extbuf_min_))
        {
            __intbuf_ = __s;
            __owns_ib_ = false;
        }
        else
        {
            __intbuf_ = new char_type[__ibs_];
            __owns_ib_ = true;
        }
    }
    else
    {
        __ibs_ = 0;
        __intbuf_ = 0;
        __owns_ib_ = false;
    }
    return this;
}

int
filebuf::sync()
{
    if (__file_ == 0)
        return 0;
    if (!__cv_) {
        // __throw_bad_cast();
    	throw std::exception();
    }

    if (__cm_ & std::ios_base::out)
    {
        if (this->pptr() != this->pbase())
            if (overflow() == traits_type::eof())
                return -1;
        std::codecvt_base::result __r;
        do
        {
            char* __extbe;
            __r = __cv_->unshift(__st_, __extbuf_, __extbuf_ + __ebs_, __extbe);
            size_t __nmemb = static_cast<size_t>(__extbe - __extbuf_);
            if (fwrite(__extbuf_, 1, __nmemb, __file_) != __nmemb)
                return -1;
        } while (__r == std::codecvt_base::partial);
        if (__r == std::codecvt_base::error)
            return -1;
        if (fflush(__file_))
            return -1;
    }
    else if (__cm_ & std::ios_base::in)
    {
        off_type __c;
        mbstate_t __state = __st_last_;
        bool __update_st = false;
        if (__always_noconv_)
            __c = this->egptr() - this->gptr();
        else
        {
            int __width = __cv_->encoding();
            __c = __extbufend_ - __extbufnext_;
            if (__width > 0)
                __c += __width * (this->egptr() - this->gptr());
            else
            {
                if (this->gptr() != this->egptr())
                {
                    const int __off =  __cv_->length(__state, __extbuf_,
                                                     __extbufnext_,
                                                     this->gptr() - this->eback());
                    __c += __extbufnext_ - __extbuf_ - __off;
                    __update_st = true;
                }
            }
        }
#if defined(_WIN32) || defined(_NEWLIB_VERSION)
        if (fseek(__file_, -__c, SEEK_CUR))
            return -1;
#else
        if (fseeko(__file_, -__c, SEEK_CUR))
            return -1;
#endif
        if (__update_st)
            __st_ = __state;
        __extbufnext_ = __extbufend_ = __extbuf_;
        this->setg(0, 0, 0);
        __cm_ = 0;
    }
    return 0;
}


void
filebuf::imbue(const std::locale& __loc)
{
    sync();
    __cv_ = &std::use_facet<std::codecvt<char_type, char, mbstate_t> >(__loc);
    bool __old_anc = __always_noconv_;
    __always_noconv_ = __cv_->always_noconv();
    if (__old_anc != __always_noconv_)
    {
        this->setg(0, 0, 0);
        this->setp(0, 0);
        // invariant, char_type is char, else we couldn't get here
        if (__always_noconv_)  // need to dump __intbuf_
        {
            if (__owns_eb_)
                delete [] __extbuf_;
            __owns_eb_ = __owns_ib_;
            __ebs_ = __ibs_;
            __extbuf_ = (char*)__intbuf_;
            __ibs_ = 0;
            __intbuf_ = 0;
            __owns_ib_ = false;
        }
        else  // need to obtain an __intbuf_.
        {     // If __extbuf_ is user-supplied, use it, else new __intbuf_
            if (!__owns_eb_ && __extbuf_ != __extbuf_min_)
            {
                __ibs_ = __ebs_;
                __intbuf_ = (char_type*)__extbuf_;
                __owns_ib_ = false;
                __extbuf_ = new char[__ebs_];
                __owns_eb_ = true;
            }
            else
            {
                __ibs_ = __ebs_;
                __intbuf_ = new char_type[__ibs_];
                __owns_ib_ = true;
            }
        }
    }
}


bool
filebuf::__read_mode()
{
    if (!(__cm_ & std::ios_base::in))
    {
        this->setp(0, 0);
        if (__always_noconv_)
            this->setg((char_type*)__extbuf_,
                       (char_type*)__extbuf_ + __ebs_,
                       (char_type*)__extbuf_ + __ebs_);
        else
            this->setg(__intbuf_, __intbuf_ + __ibs_, __intbuf_ + __ibs_);
        __cm_ = std::ios_base::in;
        return true;
    }
    return false;
}


void
filebuf::__write_mode()
{
    if (!(__cm_ & std::ios_base::out))
    {
        this->setg(0, 0, 0);
        if (__ebs_ > sizeof(__extbuf_min_))
        {
            if (__always_noconv_)
                this->setp((char_type*)__extbuf_,
                           (char_type*)__extbuf_ + (__ebs_ - 1));
            else
                this->setp(__intbuf_, __intbuf_ + (__ibs_ - 1));
        }
        else
            this->setp(0, 0);
        __cm_ = std::ios_base::out;
    }
}

typename filebuf::int_type
filebuf::underflow()
{
    if (__file_ == 0)
        return traits_type::eof();
    bool __initial = __read_mode();
    char_type __1buf;
    if (this->gptr() == 0)
        this->setg(&__1buf, &__1buf+1, &__1buf+1);
    const size_t __unget_sz = __initial ? 0 : std::min<size_t>((this->egptr() - this->eback()) / 2, 4);
    int_type __c = traits_type::eof();
    if (this->gptr() == this->egptr())
    {
        memmove(this->eback(), this->egptr() - __unget_sz, __unget_sz * sizeof(char_type));
        if (__always_noconv_)
        {
            size_t __nmemb = static_cast<size_t>(this->egptr() - this->eback() - __unget_sz);
            __nmemb = fread(this->eback() + __unget_sz, 1, __nmemb, __file_);
            if (__nmemb != 0)
            {
                this->setg(this->eback(),
                           this->eback() + __unget_sz,
                           this->eback() + __unget_sz + __nmemb);
                __c = traits_type::to_int_type(*this->gptr());
            }
        }
        else
        {
            _LIBCPP_ASSERT ( !(__extbufnext_ == NULL && (__extbufend_ != __extbufnext_)), "underflow moving from NULL" );
            if (__extbufend_ != __extbufnext_)
                memmove(__extbuf_, __extbufnext_, __extbufend_ - __extbufnext_);
            __extbufnext_ = __extbuf_ + (__extbufend_ - __extbufnext_);
            __extbufend_ = __extbuf_ + (__extbuf_ == __extbuf_min_ ? sizeof(__extbuf_min_) : __ebs_);
            size_t __nmemb = _VSTD::min(static_cast<size_t>(__ibs_ - __unget_sz),
                                 static_cast<size_t>(__extbufend_ - __extbufnext_));
            std::codecvt_base::result __r;
            __st_last_ = __st_;
            size_t __nr = fread((void*)__extbufnext_, 1, __nmemb, __file_);
            if (__nr != 0)
            {
                if (!__cv_) {
                    //__throw_bad_cast();
                	throw std::exception();
                }

                __extbufend_ = __extbufnext_ + __nr;
                char_type*  __inext;
                __r = __cv_->in(__st_, __extbuf_, __extbufend_, __extbufnext_,
                                       this->eback() + __unget_sz,
                                       this->eback() + __ibs_, __inext);
                if (__r == std::codecvt_base::noconv)
                {
                    this->setg((char_type*)__extbuf_, (char_type*)__extbuf_, (char_type*)__extbufend_);
                    __c = traits_type::to_int_type(*this->gptr());
                }
                else if (__inext != this->eback() + __unget_sz)
                {
                    this->setg(this->eback(), this->eback() + __unget_sz, __inext);
                    __c = traits_type::to_int_type(*this->gptr());
                }
            }
        }
    }
    else
        __c = traits_type::to_int_type(*this->gptr());
    if (this->eback() == &__1buf)
        this->setg(0, 0, 0);
    return __c;
}


typename filebuf::int_type
filebuf::pbackfail(int_type __c)
{
    if (__file_ && this->eback() < this->gptr())
    {
        if (traits_type::eq_int_type(__c, traits_type::eof()))
        {
            this->gbump(-1);
            return traits_type::not_eof(__c);
        }
        if ((__om_ & std::ios_base::out) ||
            traits_type::eq(traits_type::to_char_type(__c), this->gptr()[-1]))
        {
            this->gbump(-1);
            *this->gptr() = traits_type::to_char_type(__c);
            return __c;
        }
    }
    return traits_type::eof();
}


typename filebuf::int_type
filebuf::overflow(int_type __c)
{
    if (__file_ == 0)
        return traits_type::eof();
    __write_mode();
    char_type __1buf;
    char_type* __pb_save = this->pbase();
    char_type* __epb_save = this->epptr();
    if (!traits_type::eq_int_type(__c, traits_type::eof()))
    {
        if (this->pptr() == 0)
            this->setp(&__1buf, &__1buf+1);
        *this->pptr() = traits_type::to_char_type(__c);
        this->pbump(1);
    }
    if (this->pptr() != this->pbase())
    {
        if (__always_noconv_)
        {
            size_t __nmemb = static_cast<size_t>(this->pptr() - this->pbase());
            if (fwrite(this->pbase(), sizeof(char_type), __nmemb, __file_) != __nmemb)
                return traits_type::eof();
        }
        else
        {
            char* __extbe = __extbuf_;
            std::codecvt_base::result __r;
            do
            {
                if (!__cv_) {
                    //__throw_bad_cast();
                	throw std::exception();
                }

                const char_type* __e;
                __r = __cv_->out(__st_, this->pbase(), this->pptr(), __e,
                                        __extbuf_, __extbuf_ + __ebs_, __extbe);
                if (__e == this->pbase())
                    return traits_type::eof();
                if (__r == std::codecvt_base::noconv)
                {
                    size_t __nmemb = static_cast<size_t>(this->pptr() - this->pbase());
                    if (fwrite(this->pbase(), 1, __nmemb, __file_) != __nmemb)
                        return traits_type::eof();
                }
                else if (__r == std::codecvt_base::ok || __r == std::codecvt_base::partial)
                {
                    size_t __nmemb = static_cast<size_t>(__extbe - __extbuf_);
                    if (fwrite(__extbuf_, 1, __nmemb, __file_) != __nmemb)
                        return traits_type::eof();
                    if (__r == std::codecvt_base::partial)
                    {
                        this->setp((char_type*)__e, this->pptr());
                        this->pbump(this->epptr() - this->pbase());
                    }
                }
                else
                    return traits_type::eof();
            } while (__r == std::codecvt_base::partial);
        }
        this->setp(__pb_save, __epb_save);
    }
    return traits_type::not_eof(__c);
}


typename filebuf::pos_type
filebuf::seekoff(off_type __off, std::ios_base::seekdir __way,
                                        std::ios_base::openmode)
{
    if (!__cv_) {
        //__throw_bad_cast();
    	throw std::exception();
    }

    int __width = __cv_->encoding();
    if (__file_ == 0 || (__width <= 0 && __off != 0) || sync())
        return pos_type(off_type(-1));
    // __width > 0 || __off == 0
    int __whence;
    switch (__way)
    {
    case std::ios_base::beg:
        __whence = SEEK_SET;
        break;
    case std::ios_base::cur:
        __whence = SEEK_CUR;
        break;
    case std::ios_base::end:
        __whence = SEEK_END;
        break;
    default:
        return pos_type(off_type(-1));
    }
#if defined(_WIN32) || defined(_NEWLIB_VERSION)
    if (fseek(__file_, __width > 0 ? __width * __off : 0, __whence))
        return pos_type(off_type(-1));
    pos_type __r = ftell(__file_);
#else
    if (fseeko(__file_, __width > 0 ? __width * __off : 0, __whence))
        return pos_type(off_type(-1));
    pos_type __r = ftello(__file_);
#endif
    __r.state(__st_);
    return __r;
}


typename filebuf::pos_type
filebuf::seekpos(pos_type __sp, std::ios_base::openmode)
{
    if (__file_ == 0 || sync())
        return pos_type(off_type(-1));
#if defined(_WIN32) || defined(_NEWLIB_VERSION)
    if (fseek(__file_, __sp, SEEK_SET))
        return pos_type(off_type(-1));
#else
    if (fseeko(__file_, __sp, SEEK_SET))
        return pos_type(off_type(-1));
#endif
    __st_ = __sp.state();
    return __sp;
}

filebuf*
filebuf::close()
{
    filebuf* __rt = 0;
    if (__file_)
    {
        __rt = this;
        std::unique_ptr<FILE, int(*)(FILE*)> __h(__file_, fclose);
        if (sync())
            __rt = 0;
        if (fclose(__h.release()) == 0)
        {
            __file_ = 0;
            __file_fd_ = -1;
        }
        else
        {
            __rt = 0;
        }
    }
    return __rt;
}

// ifstream

ifstream::ifstream()
    : std::istream(&__sb_)
{
}

#ifndef _LIBCPP_HAS_NO_GLOBAL_FILESYSTEM_NAMESPACE

ifstream::ifstream(const char* __s, std::ios_base::openmode __mode)
	: std::istream(&__sb_)
{
    if (__sb_.open(__s, __mode | std::ios_base::in) == 0)
        this->setstate(ios_base::failbit);
}


ifstream::ifstream(const std::string& __s, std::ios_base::openmode __mode)
    : std::istream(&__sb_)
{
    if (__sb_.open(__s, __mode | std::ios_base::in) == 0)
        this->setstate(ios_base::failbit);
}
#endif

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES


ifstream::ifstream(ifstream&& __rhs)
    : std::istream(_VSTD::move(__rhs)),
      __sb_(_VSTD::move(__rhs.__sb_))
{
    this->set_rdbuf(&__sb_);
}


ifstream&
ifstream::operator=(ifstream&& __rhs)
{
    std::istream::operator=(_VSTD::move(__rhs));
    __sb_ = _VSTD::move(__rhs.__sb_);
    return *this;
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES


void
ifstream::swap(ifstream& __rhs)
{
    std::istream::swap(__rhs);
    __sb_.swap(__rhs.__sb_);
}


inline _LIBCPP_INLINE_VISIBILITY
void
swap(ifstream& __x, ifstream& __y)
{
    __x.swap(__y);
}


filebuf*
ifstream::rdbuf() const
{
    return const_cast<filebuf*>(&__sb_);
}


bool
ifstream::is_open() const
{
    return __sb_.is_open();
}

#ifndef _LIBCPP_HAS_NO_GLOBAL_FILESYSTEM_NAMESPACE

void
ifstream::open(const char* __s, std::ios_base::openmode __mode)
{
    if (__sb_.open(__s, __mode | std::ios_base::in))
        this->clear();
    else
        this->setstate(ios_base::failbit);
}


void
ifstream::open(const std::string& __s, std::ios_base::openmode __mode)
{
    if (__sb_.open(__s, __mode | std::ios_base::in))
        this->clear();
    else
        this->setstate(ios_base::failbit);
}
#endif


void
ifstream::close()
{
    if (__sb_.close() == 0)
        this->setstate(ios_base::failbit);
}

// ofstream

ofstream::ofstream()
    : std::ostream(&__sb_)
{
}

#ifndef _LIBCPP_HAS_NO_GLOBAL_FILESYSTEM_NAMESPACE

ofstream::ofstream(const char* __s, std::ios_base::openmode __mode)
    : std::ostream(&__sb_)
{
    if (__sb_.open(__s, __mode | std::ios_base::out) == 0)
        this->setstate(ios_base::failbit);
}


ofstream::ofstream(const std::string& __s, std::ios_base::openmode __mode)
    : std::ostream(&__sb_)
{
    if (__sb_.open(__s, __mode | std::ios_base::out) == 0)
        this->setstate(ios_base::failbit);
}
#endif

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES


ofstream::ofstream(ofstream&& __rhs)
    : std::ostream(_VSTD::move(__rhs)),
      __sb_(_VSTD::move(__rhs.__sb_))
{
    this->set_rdbuf(&__sb_);
}


ofstream&
ofstream::operator=(ofstream&& __rhs)
{
	std::ostream::operator=(_VSTD::move(__rhs));
    __sb_ = _VSTD::move(__rhs.__sb_);
    return *this;
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES


void
ofstream::swap(ofstream& __rhs)
{
	std::ostream::swap(__rhs);
    __sb_.swap(__rhs.__sb_);
}


inline _LIBCPP_INLINE_VISIBILITY
void
swap(ofstream& __x, ofstream& __y)
{
    __x.swap(__y);
}


filebuf*
ofstream::rdbuf() const
{
    return const_cast<filebuf*>(&__sb_);
}


bool
ofstream::is_open() const
{
    return __sb_.is_open();
}

#ifndef _LIBCPP_HAS_NO_GLOBAL_FILESYSTEM_NAMESPACE

void
ofstream::open(const char* __s, std::ios_base::openmode __mode)
{
    if (__sb_.open(__s, __mode | std::ios_base::out))
        this->clear();
    else
        this->setstate(ios_base::failbit);
}


void
ofstream::open(const std::string& __s, std::ios_base::openmode __mode)
{
    if (__sb_.open(__s, __mode | std::ios_base::out))
        this->clear();
    else
        this->setstate(ios_base::failbit);
}
#endif


void
ofstream::close()
{
    if (__sb_.close() == 0)
        this->setstate(ios_base::failbit);
}

// fstream

fstream::fstream()
    : std::iostream(&__sb_)
{
}

#ifndef _LIBCPP_HAS_NO_GLOBAL_FILESYSTEM_NAMESPACE

fstream::fstream(const char* __s, std::ios_base::openmode __mode)
    : std::iostream(&__sb_)
{
    if (__sb_.open(__s, __mode) == 0)
        this->setstate(ios_base::failbit);
}


fstream::fstream(const std::string& __s, std::ios_base::openmode __mode)
    : std::iostream(&__sb_)
{
    if (__sb_.open(__s, __mode) == 0)
        this->setstate(ios_base::failbit);
}
#endif

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES


fstream::fstream(fstream&& __rhs)
    : std::iostream(_VSTD::move(__rhs)),
      __sb_(_VSTD::move(__rhs.__sb_))
{
    this->set_rdbuf(&__sb_);
}


fstream&
fstream::operator=(fstream&& __rhs)
{
    std::iostream::operator=(_VSTD::move(__rhs));
    __sb_ = _VSTD::move(__rhs.__sb_);
    return *this;
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES


void
fstream::swap(fstream& __rhs)
{
    std::iostream::swap(__rhs);
    __sb_.swap(__rhs.__sb_);
}


inline _LIBCPP_INLINE_VISIBILITY
void
swap(fstream& __x, fstream& __y)
{
    __x.swap(__y);
}


filebuf*
fstream::rdbuf() const
{
    return const_cast<filebuf*>(&__sb_);
}


bool
fstream::is_open() const
{
    return __sb_.is_open();
}

#ifndef _LIBCPP_HAS_NO_GLOBAL_FILESYSTEM_NAMESPACE

void
fstream::open(const char* __s, std::ios_base::openmode __mode)
{
    if (__sb_.open(__s, __mode))
        this->clear();
    else
        this->setstate(ios_base::failbit);
}


void
fstream::open(const std::string& __s, std::ios_base::openmode __mode)
{
    if (__sb_.open(__s, __mode))
        this->clear();
    else
        this->setstate(ios_base::failbit);
}
#endif


void
fstream::close()
{
    if (__sb_.close() == 0)
        this->setstate(ios_base::failbit);
}
