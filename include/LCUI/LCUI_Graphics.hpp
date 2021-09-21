/* ***************************************************************************
 * LCUI_Graphics.hpp -- C++ class of graphics processing
 * 
 * Copyright (C) 2012 by
 * Liu Chao
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 * 
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *  
 * The LCUI project is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 * 
 * You should have received a copy of the GPLv2 along with this file. It is 
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/
 
/* ****************************************************************************
 * LCUI_Graphics.hpp -- ͼ�δ����C++��
 *
 * ��Ȩ���� (C) 2012 ������ 
 * ����
 * 
 * ����ļ���LCUI��Ŀ��һ���֣�����ֻ���Ը���GPLv2���Э����ʹ�á����ĺͷ�����
 *
 * (GPLv2 �� GNUͨ�ù������֤�ڶ��� ��Ӣ����д)
 * 
 * ����ʹ�á��޸Ļ򷢲����ļ����������Ѿ��Ķ�����ȫ���ͽ���������Э�顣
 * 
 * LCUI ��Ŀ�ǻ���ʹ��Ŀ�Ķ�����ɢ���ģ��������κε������Σ�����û�������Ի���
 * ����;���������������������GPLv2���Э�顣
 *
 * ��Ӧ���յ������ڱ��ļ���GPLv2���Э��ĸ�������ͨ����LICENSE.TXT�ļ��У����
 * û�У���鿴��<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/
#ifndef __LCUI_GRAPHICS_HPP__
#define __LCUI_GRAPHICS_HPP__
#include LC_MEM_H
#include LC_RES_H

#ifdef __cplusplus

class LCUIGraph
{
	public:
	LCUI_Graph graph;
	
	LCUIGraph():load(this)
	{
		Graph_Init(&graph); 
	}
	~LCUIGraph()
	{
		Free_Graph(&graph);
	}
	int isValid()
	{
		return Valid_Graph(&graph);
	}
	int haveAlpha()
	{
		return Graph_Have_Alpha(&graph);
	}
	int loadFromFile(const char *in_imgfile)
	{
		return Load_Image(in_imgfile, &graph);
	}
	int writeToFile(const char *out_imgfile)
	{
		return write_png_file(out_imgfile, &graph);
	} 
	
	class _loadGraph
	{
		public:
		LCUI_Graph *graph;
		_loadGraph(LCUIGraph *object):icon(this)
		{
			graph = &object->graph; 
		}
		class _icon
		{
			public:
			_icon(_loadGraph *object)
			{
				graph = &object->graph; 
			}
			void LCUI_18x18()
			{
				Load_Graph_Icon_LCUI_18x18(*graph);
			}
			private:
			LCUI_Graph **graph; 
		};
		
		_icon icon; 
	};
	_loadGraph load;
};
#endif

#endif
