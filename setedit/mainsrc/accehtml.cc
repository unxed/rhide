/* Copyright (C) 1996-2003 by Salvador E. Tropea (SET),
   see copyrigh file for details */
#define Uses_string

#define Uses_TView
#define Uses_TApplication
#define Uses_TDeskTop
#define Uses_MsgBox
#define Uses_TCEditWindow
#define Uses_TVCodePage
#include <ceditor.h>
#define Uses_SETAppVarious

#include <setapp.h>
#include <ctype.h>
#include <ucdefs.h>

static void ExpandAccents(TCEditor *e);
static void CompactAccents(TCEditor *e);

void HTMLAcc_Entry(Boolean compact)
{
 TView *p;

 p=TApplication::deskTop->current;
 if (!p) return; // Avoid a search
 if (IsAnEditor(p))
   {
    TCEditor *e=((TCEditWindow *)p)->editor;
    if (e->isReadOnly || TCEditor::editorDialog(edActionWOUndo)!=cmYes)
       return;
    if (e->IslineInEdition)
       e->MakeEfectiveLineInEdition();
    e->buffer[e->bufLen]=0;
    e->SetMarker(9);

    if (compact)
       CompactAccents(e);
    else
       ExpandAccents(e);

    /* Jump to the start because the line offset is no longer valid and it can
       mess the drawing */
    e->ResetCursorPosition();
    e->GotoOffSet(e->Markers[9]);
    e->update(ufView);
    e->trackCursor(True);

    /* Flush the undo information */
    e->flushUndoInfo();
   }
 else
   messageBox(__("You must select the editor window first"),mfError | mfOKButton);
}

/*****************************************************************************

  The following table translates my internal fonts numbers to ISO-1 html
strings.

*****************************************************************************/
static char *Remap[]=
{
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 0..15
 0,0,0,0,
 "para",   // 20 � pilcrow (paragraph sign)
 "sect",   // 21 � section sign
 0,0,0,0,0,0,0,0,0,0,             // ..31
 0,0,0,0,
 "curren", // 36 � general currency sign
 0,0,0,0,0,0,0,0,0,0,0,           // ..47
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // ..127
 "Ccedil", // 128 capital C, cedilla
 "uuml",   // 129 small u, dieresis or umlaut mark
 "eacute", // 130 � small e, acute accent
 "acirc",  // 131 small a, circumflex accent
 "auml",   // 132 � mall a, dieresis or umlaut mark 
 "agrave", // 133 small a, grave accent
 "aring",  // 134 small a, ring
 "ccedil", // 135 � small c, cedilla
 "ecirc",  // 136 � small e, circumflex accent
 "euml",   // 137 � small e, dieresis or umlaut mark
 "egrave", // 138 � small e, grave accent
 "iuml",   // 139 � small i, dieresis or umlaut mark
 "icirc",  // 140 � small i, circumflex accent
 "igrave", // 141 � small i, grave accent
 "Auml",   // 142 � capital A, dieresis or umlaut mark
 "Aring",  // 143 � capital A, ring
 "Eacute", // 144 � capital E, acute accent
 "aelig",  // 145 small ae diphthong (ligature)
 "AElig",  // 146 capital AE diphthong (ligature)
 "ocirc",  // 147 � small o, circumflex accent
 "ouml",   // 148 � small o, dieresis or umlaut mark
 "ograve", // 149 � small o, grave accent
 "ucirc",  // 150 � small u, circumflex accent
 "ugrave", // 151 � small u, grave accent
 "yuml",   // 152 small y, dieresis or umlaut mark
 "Ouml",   // 153 � capital O, dieresis or umlaut mark
 "Uuml",   // 154 � capital U, dieresis or umlaut mark
 "cent",   // 155 cent sign
 "pound",  // 156 pound sterling sign
 "yen",    // 157 yen sign
 0,        // 158 peseta
 0,        // 159 flor�n
 "aacute", // 160 � small a, acute accent
 "iacute", // 161 � small i, acute accent
 "oacute", // 162 � small o, acute accent
 "uacute", // 163 � small u, acute accent
 "ntilde", // 164 small n, tilde
 "Ntilde", // 165 capital N, tilde
 "ordf",   // 166 � ordinal indicator, feminine
 "ordm",   // 167 � ordinal indicator, masculine
 "iquest", // 168 � inverted question mark
 0,        // 169
 "not",    // 170 � not sign
 "frac12", // 171 � fraction one-half
 "frac14", // 172 � fraction one-quarter
 "iexcl",  // 173 � inverted exclamation mark
 "laquo",  // 174 � angle quotation mark, left
 "raquo",  // 175 � angle quotation mark, right
 0,0,0,0,0,           // 176 .. 180
 0,0,0,0,0,0,0,0,0,0, // 181 .. 190
 0,0,0,0,0,0,0,0,0,0, // 191 .. 200
 0,0,0,0,0,0,0,0,0,0, // 201 .. 210
 0,0,0,0,0,0,0,0,0,0, // 211 .. 220
 0,0,0,
 0,        // 224 alfa
 "szlig",  // 225 � small sharp s, German (sz ligature)
 0,        // 226 gama may.
 0,        // 227 pi
 0,        // 228 sigma may.
 0,        // 229 sigma
 "micro",  // 230 � micro sign
 0,        // 231 tau
 0,0,0,0,0,0,0,0,0,
 "plusmn", // 241 � plus-or-minus sign
 0,0,0,0,
 "divide", // 246 � divide sign
 0,
 "deg",    // 248 � degree sign
 "middot", // 249 � middle dot
 0,0,0,
 "sup2",   // 253 � superscript two
 0,0,      // ..255
 0,0,0,0,0,
 "yacute", // 261 � small y, acute accent
 0,
 "Aacute", // 263 � capital A, acute accent
 0,
 "Iacute", // 265 � capital I, acute accent
 0,0,
 "Oacute", // 268 � capital O, acute accent
 0,0,
 "Uacute", // 271 � capital U, acute accent
 "Yacute", // 272 � capital Y, acute accent
 0,0,
 "Agrave", // 275 � capital A, grave accent
 "Egrave", // 276 � capital E, grave accent
 "Igrave", // 277 � capital I, grave accent
 "Ograve", // 278 � capital O, grave accent
 "Ugrave", // 279 � capital U, grave accent
 "Euml",   // 280 � capital E, dieresis or umlaut mark
 "Iuml",   // 281 � capital I, dieresis or umlaut mark
 0,0,
 "atilde", // 284 � small a, tilde
 "otilde", // 285 � small o, tilde
 0,
 "Atilde", // 287 � capital A, tilde
 "Otilde", // 288 � capital O, tilde
 "Acirc",  // 289 � capital A, circumflex accent
 "Ecirc",  // 290 � capital E, circumflex accent
 "Icirc",  // 291 � capital I, circumflex accent
 "Ocirc",  // 292 � capital O, circumflex accent
 "Ucirc",  // 293 � capital U, circumflex accent
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 "oslash", // 318 � small o, slash
 0,
 "Oslash", // 320 � capital O, slash
 "eth",    // 321 � small eth, Icelandic
 0,
 "ETH",    // 323 � capital Eth, Icelandic
 0,0,0,0,0,0,0,0,
 "thorn",  // 332 � small thorn, Icelandic
 "THORN",  // 333 � capital THORN, Icelandic
 "times",  // 334 � multiply sign
 "reg",    // 335 � registered sign
 "copy",   // 336 � copyrigh\t sign
 0,
 "brvbar", // 338 � broken (vertical) bar
 0,
 "acute",  // 340 � acute accent
 "shy",    // 341 � soft hyphen /// macron
 0,
 "frac34", // 343 � fraction three-quarters
 "cedil",  // 344 � cedilla
 "uml",    // 345 � umlaut (dieresis)
 "sup1",   // 346 � superscript one
 "sup3"    // 347 � superscript three
};

const unsigned LastRemap=sizeof(Remap)/sizeof(char *);

static
void ExpandAccents(TCEditor *e)
{
 // Get the map for the current code page
 ushort *map=TVCodePage::GetTranslate(TVCodePage::GetAppCodePage());

 e->lock();
 e->lockUndo();

 uchar *s=(uchar *)e->buffer;
 char *repl;
 unsigned len=e->bufLen,pos=0,value,index;
 char buf[12];
 buf[0]='&';

 while (pos<len)
   {
    // Convert the char code to the internal representation
    value=map[s[pos]];
    if (value<LastRemap)
      {
       if (Remap[value])
         {
          e->deleteRange((char *)s+pos,(char *)s+pos+1,False);
          repl=Remap[value];
          for (index=0; repl[index]; index++)
              buf[index+1]=repl[index];
          buf[index+1]=';';
          e->insertText(buf,index+2,False);
          // Adjust the len because it changed
          len=e->bufLen;
          // The buffer could be reallocated and hence changed!
          s=(uchar *)e->buffer;
         }
      }
    pos++;
   }
 e->unlockUndo();
 e->unlock();
}

static
void CompactAccents(TCEditor *e)
{
 // Get the map for the current code page
 ushort *map=TVCodePage::GetTranslate(TVCodePage::GetAppCodePage());

 e->lock();
 e->lockUndo();

 uchar *s=(uchar *)e->buffer;
 unsigned len=e->bufLen,pos=0,value,index,lenW,posStart;
 char buf[12];

 while (pos<len)
   {
    if (s[pos]!='&')
      {
       pos++;
       continue;
      }
    posStart=pos;
    // Take the word
    index=0; pos++;
    while (pos<len && TVCodePage::isAlpha(s[pos]) && index<6)
      {
       buf[index++]=s[pos];
       pos++;
      }
    lenW=index+1;
    buf[index]=0;
    if (pos<len)
      {
       if (TVCodePage::isAlpha(s[pos]))
          continue;
       if (s[pos]==';')
          lenW++;
      }

    // Search what's the code for it
    // Could be faster creating a sorted collection with the names.
    for (value=0; value<LastRemap; value++)
        if (Remap[value] && strcmp(Remap[value],buf)==0)
           break;
    if (value==LastRemap) continue;

    // Search the code in this code page
    // Could be faster creating a look-up table
    for (index=0; index<256; index++)
        if ((unsigned)map[index]==value)
           break;
    if (index==256) continue;

    e->deleteRange((char *)s+posStart,(char *)s+posStart+lenW,False);
    buf[0]=index;
    e->insertText(buf,1,False);
    pos-=lenW-1;
    // Adjust the len because it changed
    len=e->bufLen;
    // The buffer could be reallocated and hence changed!
    s=(uchar *)e->buffer;
   }
 e->unlockUndo();
 e->unlock();
}


