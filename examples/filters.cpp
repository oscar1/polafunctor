#include "../inc/polafunctor.hpp"
#include <string>
#include <iostream>

class logger: public polafunctor::functor<int, std::string, int, std::string> {
     std::ostream &mStream;
     int mMinLevel;
  public:
     logger(std::ostream &stream,int minlevel):mStream(stream),mMinLevel(minlevel){
        if (mMinLevel<1) {
          mMinLevel=1;
        }
        if (mMinLevel > 6) {
          mMinLevel=6;
        }
     }
     int operator()(std::string prefix,int level, std::string message) {
        if (level >= mMinLevel) {
          std::string slevel("INVALID_LEVEL");
          switch (level) {
             case 1: slevel="DEBUG";break;
             case 2: slevel="INFO" ;break;
             case 3: slevel="NOTICE";break;
             case 4: slevel="WARNING"; break;
             case 5: slevel="ERROR"; break;
          }
          mStream << "[" << prefix << "]:" << slevel << " : " << message << std::endl;
        }
     }
};

class namespacefilter: public polafunctor::functor<std::string,std::string> {
    std::string mNamespace;
  public:
    namespacefilter(std::string filterns):mNamespace(filterns) {}
    std::string operator()(std::string input) {
       return mNamespace + ":" + input;
    }
};

int main(int argc,char **argv) {
  logger rawlogger(std::cerr,2);
  rawlogger("sapperdeflap",44,"repelsteeltje");
  namespacefilter nsfilter("foo");
  polafunctor::filter_first_argument<int, std::string, int, std::string> nslogger(rawlogger,nsfilter);
  nslogger("sapperdeflap",44,"repelsteeltje");
  polafunctor::argumentstore<std::string> firstargument("uninitialized");
  polafunctor::auto_first_argument<int, std::string, int, std::string> logger1(nslogger,firstargument);
  polafunctor::range_filter<int> twotofour(2,4);
  polafunctor::filter_first_argument<int,int, std::string> logger2(logger1,twotofour);
  polafunctor::redirected_first_argument<int, std::string, int, std::string> filteredlogger(logger2,firstargument);
  filteredlogger("sapperdeflap",44,"repelsteeltje");       
  return 0;
}
