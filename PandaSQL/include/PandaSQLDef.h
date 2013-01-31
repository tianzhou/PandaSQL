#ifndef PANDASQL_DEF_H
#define PANDASQL_DEF_H

#ifdef PANDASQL_LIB
#define PANDASQL_API __declspec( dllexport )
#else
#define PANDASQL_API __declspec( dllimport )
#endif

namespace PandaSQL
{

struct PANDASQL_API OpenOptions
{
	bool create_if_missing;

	OpenOptions();
};

}	// namespace PandaSQL

#endif	// PANDASQL_DEF_H