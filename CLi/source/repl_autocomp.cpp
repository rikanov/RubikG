#include <cli.h>
#include <stdio.h>
#include <vector>
#include <readline/readline.h>

char ** CLi::commandCompletion( const char * stem_text,int start,int end ) 
{
  char ** matches = NULL;
  matches = rl_completion_matches(stem_text,getCommands);
  return matches ;
}


char * CLi::getCommands (const char * stem_text,int state) 
{
  static std::vector < const char * > intersection_names = {
 "Yonge Street & Eglinton Avenue East & Eglinton Avenue West ",
 "Bay Street & Dundas Street West ",
 "Bay Street & Wellington Street West ",
 "Yonge Street & Dundas Street West & Dundas Street East ",
 "Yonge Street & College Street & Carlton Street ",
 "Yonge Street & Queen Street West & Queen Street East ",
 "Bloor Street West & Bay Street ",
 "University Avenue & Front Street West & York Street ",
 "King Street West & Bay Street ",
 "St. George Street & Harbord Street & Hoskin Avenue ",
 "College Street & Bay Street ",
 "Queen Street West & John Street ",
 "Avenue Road & Bloor Street West & Queen ’s Park ",
 "Front Street West & John Street & Convention centre loading dock ",
 "University Avenue & Wellington Street West "
  };

 static int count ;

 if( state == 0) 
 {
   // We initialize the count the first time we are called
   // with this stem_text
   count = -1;
 }

 int text_len = strlen ( stem_text ) ;

 // Search through intersection_names until we find a match
 while ( count < (int) intersection_names . size () -1) 
 {
   ++count;
   if( strncmp ( intersection_names [ count ] , stem_text , text_len ) == 0) 
   {
     // Must return a duplicate , Readline will handle
     // freeing this string itself .
     return strdup ( intersection_names [ count ]) ;
   }
 }

 // No more matches
  return nullptr;
}
