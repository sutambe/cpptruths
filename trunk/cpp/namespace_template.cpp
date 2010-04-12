namespace PICML
{
  struct RootFolder {};
  struct Component {};  
  struct Port {};
  struct PICML_Lang {};
}

namespace CQML 
{
  struct RootFolder {};
  struct Component {};  
  struct Port {};
  struct CQML_Lang {};
}

template <typename MLang>
struct MLTraits;

template <>
struct MLTraits <PICML::PICML_Lang>
{
  typedef PICML::RootFolder RootFolder;
  typedef PICML::Component Component;
};

template <>
struct MLTraits <CQML::CQML_Lang>
{
  typedef CQML::RootFolder RootFolder;
  typedef CQML::Component Component;
};

template <typename MLang>
void gen_deployment_plan (typename MLTraits<MLang>::RootFolder const &)
{
}

int main(void)
{
  PICML::RootFolder prf;
  CQML::RootFolder crf;
  gen_deployment_plan <CQML::CQML_Lang> (crf); 
  gen_deployment_plan <PICML::PICML_Lang> (prf); 
  return 0;
}

