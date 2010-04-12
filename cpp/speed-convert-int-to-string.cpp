#ifdef _MSC_VER
    #define _SECURE_SCL 0
    #define _CRT_SECURE_NO_DEPRECATE 1
    #define WIN32_LEAN_AND_MEAN
    #define VC_EXTRALEAN
    #define NOMINMAX
#endif

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <vector>
#include <iomanip>
#include <sstream>
#include <strstream>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/karma.hpp>
#include "cycle.h"

#ifdef _MSC_VER
    #define snprintf _snprintf
#endif

static const int N = 100000;
static const size_t R = 7;

void PrintStats(std::vector<double> timings) {
    double fastest = std::numeric_limits<double>::max();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[";
    for (size_t i = 1 ; i<timings.size()-1 ; ++i) {
        fastest = std::min(fastest, timings[i]);
        std::cout << timings[i] << ",";
    }
    std::cout << timings.back();
    std::cout << "]";

    double sum = 0.0;
    for (size_t i = 1 ; i<timings.size() ; ++i) {
        sum += timings[i];
    }
    double avg = sum / double(timings.size()-1);

    sum = 0.0;
    for (size_t i = 1 ; i<timings.size() ; ++i) {
        timings[i] = pow(timings[i]-avg, 2);
        sum += timings[i];
    }
    double var = sum/(timings.size()-2);
    double sdv = sqrt(var);

    std::cout << " with fastest " << fastest << ", average " << avg << ", stddev " << sdv;
}

int main() {
    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            char x[12];
            std::string y(12, 0);
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                sprintf(x, "%i", i);
                y = x;
                tlen += y.length();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "sprintf char[] (reuse string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            char x[12];
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                sprintf(x, "%i", i);
                std::string y(x);
                tlen += y.length();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "sprintf char[] (new string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            char x[12];
            std::string y(12, 0);
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                snprintf(x, 12, "%i", i);
                y = x;
                tlen += y.length();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "snprintf char[] (reuse string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            std::string y(12, 0);
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                y.resize(12, 0);
                y.resize(sprintf(&y[0], "%i", i));
                tlen += y.length();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "sprintf &string[0] (reuse string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            std::string y(12, 0);
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                y.resize(12, 0);
                y.resize(snprintf(&y[0], 12, "%i", i));
                tlen += y.length();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "snprintf &string[0] (reuse string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            std::string y(12, 0);
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                std::stringstream ss;
                ss << i;
                y = ss.str();
                tlen += y.length();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "stringstream (new stream, reuse string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                std::stringstream ss;
                ss << i;
                std::string y = ss.str();
                tlen += y.length();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "stringstream (new stream, new string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            std::stringstream ss;
            std::string y(12, 0);
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                ss << i;
                y = ss.str();
                tlen += y.length();
                ss.str("");
                ss.clear();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "stringstream (reuse stream, reuse string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            std::stringstream ss;
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                ss << i;
                std::string y = ss.str();
                tlen += y.length();
                ss.str("");
                ss.clear();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "stringstream (reuse stream, new string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            std::string y(12, 0);
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                std::strstream ss;
                ss << i;
                ss >> y;
                tlen += y.length();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "strstream (new stream, reuse string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            std::string y(12, 0);
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
            	std::ostrstream ss(&y[0], 12);
                ss << i;
				y.resize(ss.pcount());
                tlen += y.length();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "strstream (reuse stream, reuse string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            std::string y(12, 0);
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                y = boost::lexical_cast<std::string>(i);
                tlen += y.length();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "lexical_cast (reuse string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                std::string y = boost::lexical_cast<std::string>(i);
                tlen += y.length();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "lexical_cast (new string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        using namespace boost::spirit;
        using boost::spirit::karma::generate;
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            char x[12];
            std::string y(12, 0);
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                char *p = x;
                generate(p, int_, i);
                *p = 0;
                y = x;
                tlen += y.length();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "karma (reuse string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }

    {
        using namespace boost::spirit;
        using boost::spirit::karma::generate;
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            char x[12];
            for (int i=0-(N/2) ; i<N/2 ; ++i) {
                char *p = x;
                generate(p, int_, i);
                *p = 0;
                std::string y(x);
                tlen += y.length();
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "karma (new string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }
}
