#ifndef ABSTRACT_FOO_HPP 
#define ABSTRACT_FOO_HPP
class AbstactFoo {
  public:
    virtual ~AbstractFoo(){}
    virtual void aap(int a,std::string b,char *c)=0;
    virtual std::string noot(std::string a)=0;
    virtual int mies(std::string a)=0;
};
#endif;
