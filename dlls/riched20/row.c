/*
 * RichEdit - Operations on rows of text (rows are recreated during
 * wrapping and are used for displaying the document, they don't keep any
 * true document content; delete all rows, rewrap all paragraphs and 
 * you get them back).
 * 
 * Copyright 2004 by Krzysztof Foltman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */ 


#include "editor.h"

ME_Row *row_next( ME_Row *row )
{
    ME_DisplayItem *item;

    item = ME_FindItemFwd( row_get_di( row ), diStartRowOrParagraphOrEnd );
    if (!item || item->type != diStartRow) return NULL;
    return &item->member.row;
}

ME_Run *row_first_run( ME_Row *row )
{
    ME_DisplayItem *item;

    item = ME_FindItemFwd( row_get_di( row ), diRunOrStartRow );
    assert( item->type == diRun );
    return &item->member.run;
}

ME_Run *row_next_run( ME_Row *row, ME_Run *run )
{
    ME_DisplayItem *item;

    assert( row == &ME_FindItemBack( run_get_di( run ), diStartRow )->member.row );

    item = ME_FindItemFwd( run_get_di( run ), diRunOrStartRow );
    if (!item || item->type == diStartRow) return NULL;
    return &item->member.run;
}

ME_Row *row_from_cursor( ME_Cursor *cursor )
{
    ME_DisplayItem *item;

    item = ME_FindItemBack( cursor->pRun, diStartRow );
    return &item->member.row;
}

void row_first_cursor( ME_Row *row, ME_Cursor *cursor )
{
    ME_DisplayItem *item;

    item = ME_FindItemFwd( row_get_di( row ), diRun );
    cursor->pRun = item;
    cursor->pPara = para_get_di( cursor->pRun->member.run.para );
    cursor->nOffset = 0;
}

void row_end_cursor( ME_Row *row, ME_Cursor *cursor, BOOL include_eop )
{
    ME_DisplayItem *item, *run;

    item = ME_FindItemFwd( row_get_di( row ), diStartRowOrParagraphOrEnd );
    run = ME_FindItemBack( item, diRun );
    cursor->pRun = run;
    cursor->pPara = para_get_di( cursor->pRun->member.run.para );
    cursor->nOffset = (item->type == diStartRow || include_eop) ? cursor->pRun->member.run.len : 0;
}

/* I'm sure these functions would simplify some code in caret ops etc,
 * I just didn't remember them when I wrote that code
 */ 

ME_DisplayItem *ME_RowStart(ME_DisplayItem *item) {
  return ME_FindItemBackOrHere(item, diStartRow);
}

/*
ME_DisplayItem *ME_RowEnd(ME_DisplayItem *item) {
  ME_DisplayItem *item2 = ME_FindItemFwd(item, diStartRowOrParagraphOrEnd);
  if (!item2) return NULL;
  return ME_FindItemBack(item, diRun);
}
*/

ME_DisplayItem *
ME_FindRowWithNumber(ME_TextEditor *editor, int nRow)
{
  ME_DisplayItem *item = ME_FindItemFwd(editor->pBuffer->pFirst, diParagraph);
  int nCount = 0;

  while (item->type == diParagraph &&
         nCount + item->member.para.nRows <= nRow)
  {
    nCount += item->member.para.nRows;
    item = item->member.para.next_para;
  }
  if (item->type != diParagraph)
    return NULL;
  for (item = ME_FindItemFwd(item, diStartRow); item && nCount < nRow; nCount++)
    item = ME_FindItemFwd(item, diStartRow);
  return item;
}


int
ME_RowNumberFromCharOfs(ME_TextEditor *editor, int nOfs)
{
  ME_DisplayItem *item = ME_FindItemFwd(editor->pBuffer->pFirst, diParagraph);
  int nRow = 0;

  while (item->type == diParagraph &&
         item->member.para.next_para->member.para.nCharOfs <= nOfs)
  {
    nRow += item->member.para.nRows;
    item = item->member.para.next_para;
  }
  if (item->type == diParagraph)
  {
    ME_DisplayItem *next_para = item->member.para.next_para;

    nOfs -= item->member.para.nCharOfs;
    item = ME_FindItemFwd(item, diRun);
    while ((item = ME_FindItemFwd(item, diStartRowOrParagraph)) != NULL)
    {
      if (item == next_para)
        break;
      item = ME_FindItemFwd(item, diRun);
      if (item->member.run.nCharOfs > nOfs)
        break;
      nRow++;
    }
  }
  return nRow;
}
