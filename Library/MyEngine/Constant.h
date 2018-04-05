#ifndef __CONSTANT__
#define __CONSTANT__


#include <assert.h>

//--‰ğ•úŒn
#define R_SAFE_DELETE(X) if(X){delete X;X = nullptr;  }
#define R_SAFE_RELEASE(X) if(X){X->Release(); X = nullptr;}
#define R_SAFE_COUNT_RELEASE(X) if(X){X->CountRelease();}



//---SetterEGetterEvariable‚ğì¬
#define R_SYNTHESIZE(type,name,funcName)\
protected: type name; \
public:	   void Set##funcName(type var){ name = var; }\
public:	   type Get##funcName(void) const { return name; }\



//--- SetterEGetter‚Ì‚İì¬
#define R_SYNTHESIZE_FUNC_ONLY(type,name,funcName)\
public:	   void Set##funcName(type var){ name = var; }\
public:	   type Get##funcName(void) const { return name; }\



#define R_ASSERT(n) assert(n);



#endif //__CONSTANT__