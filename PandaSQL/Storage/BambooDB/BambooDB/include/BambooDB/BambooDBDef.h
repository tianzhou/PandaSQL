#ifndef BAMBOODB_DEF_H
#define BAMBOODB_DEF_H

#ifdef BAMBOODB_LIB
#define BAMBOODB_API __declspec( dllexport )
#else
#define BAMBOODB_API __declspec( dllimport )
#endif

namespace BambooDB
{

}	// namespace BambooDB

#endif	// BAMBOODB_DEF_H