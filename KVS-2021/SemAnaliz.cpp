#include "pch.h"
#include "IT.h"
#include "LT.h"
#include "Error.h"
#include "LexAnaliz.h"
#include "SemAnaliz.h"

namespace Semantic
{
	bool Semantic::semanticsCheck(Lexer::LEX & tables, Log::LOG & log)
	{
		bool sem_ok = true;

		for (int i = 0; i < tables.lextable.size; i++)
		{
			switch (tables.lextable.table[i].lexema)
			{
			case LEX_NEW:
			{
				if (tables.lextable.table[i + 1].lexema != LEX_ID_TYPE)
				{
					sem_ok = false;
					throw ERROR_THROW_IN(303, tables.lextable.table[i].sn, 0);
				}
			}
			case LEX_DIRSLASH:
			{
					int k = i;
					if (tables.lextable.table[i + 1].lexema == LEX_ID)
					{
						for (k; k > 0; k--)
						{
							if (tables.lextable.table[k].lexema == LEX_ID)
							{
								if (tables.idtable.table[tables.lextable.table[k].idxTI].id == tables.idtable.table[tables.lextable.table[i + 1].idxTI].id)
								{
									if (tables.lextable.table[k + 2].lexema == LEX_LITERAL && tables.idtable.table[tables.lextable.table[k + 2].idxTI].value.vint == 0)
									{
										sem_ok = false;
										throw ERROR_THROW_IN(318, tables.lextable.table[k].sn, 0);
									}
								}
							}
						}
					}
					if (tables.lextable.table[i + 1].lexema == LEX_LITERAL)
					{
						if (tables.idtable.table[tables.lextable.table[i + 1].idxTI].value.vint == 0)
						{
							sem_ok = false;
							throw ERROR_THROW_IN(318, tables.lextable.table[k].sn, 0);
						}
					}
					break;
				}
			case LEX_EQUAL: // ���������
			{
				if (i > 0 && tables.lextable.table[i - 1].idxTI != NULLIDX_TI) // ����� �������
				{
					IT::IDDATATYPE lefttype = tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype;
					bool ignore = false;

					for (int k = i + 1; tables.lextable.table[k].lexema != LEX_SEPARATOR; k++)
					{
						if (k == tables.lextable.size)
							break; // ������� ������ - ��� ;
						if (tables.lextable.table[k].idxTI != NULLIDX_TI) // ���� �� - ��������� ���������� �����
						{
							if (!ignore)
							{
								IT::IDDATATYPE righttype = tables.idtable.table[tables.lextable.table[k].idxTI].iddatatype;
								if (lefttype != righttype) // ���� ������ � ��������� �� ���������
								{
									throw ERROR_THROW_IN(314, tables.lextable.table[k].sn, 0);
									sem_ok = false;
									break;
								}
							}
							// ���� ������� ����� ����� ������� ������ - ��� ����� �������
							if (tables.lextable.table[k + 1].lexema == LEX_LEFTHESIS)
							{
								ignore = true;
								continue;
							}
							// ����������� ������ ����� ������ ����������
							if (ignore && tables.lextable.table[k + 1].lexema == LEX_RIGHTTHESIS)
							{
								ignore = false;
								continue;
							}
						}
						if (lefttype == IT::IDDATATYPE::STR) // ������ ������ �������, �� ��� ����� ��������� �-���
						{
							char l = tables.lextable.table[k].lexema;
							if (l == LEX_PLUS || l == LEX_MINUS || l == LEX_STAR || l == LEX_AND || l == LEX_OR || l == LEX_NOT) // ��������� �����������
							{
								throw ERROR_THROW_IN(316, tables.lextable.table[k].sn, 0);
								sem_ok = false;
								break;
							}
						}
					} 
				}
				break;
			}
			case LEX_ID: // �������� ���� ������������� ��������  
			{
				IT::Entry e = tables.idtable.table[tables.lextable.table[i].idxTI];

				if (i > 0 && tables.lextable.table[i - 1].lexema == LEX_FUNCTION)
				{
					if (e.idtype == IT::IDTYPE::F) //�������, �� ���������
					{
						for (int k = i + 1; ; k++)
						{
							char l = tables.lextable.table[k].lexema;
							if (l == LEX_RETURN)
							{
								int next = tables.lextable.table[k + 1].idxTI; // ����. �� return
								if (next != NULLIDX_TI)
								{
									// ��� ������� � ������������� �������� �� ���������
									if (tables.idtable.table[next].iddatatype != e.iddatatype)
									{
										throw ERROR_THROW_IN(315, tables.lextable.table[k].sn, 0);
										sem_ok = false;
										break;
									}
								}
								break; // ����� exit
							}

							if (k == tables.lextable.size) break;
						}
					}
				}
				if (tables.lextable.table[i + 1].lexema == LEX_LEFTHESIS && tables.lextable.table[i - 1].lexema != LEX_FUNCTION) // ������ �����
				{
					if (e.idtype == IT::IDTYPE::F || e.idtype == IT::IDTYPE::S) // ����� �������
					{
						int paramscount = NULL;
						// �������� ������������ ����������
						for (int j = i + 1; tables.lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)
						{
							// �������� ������������ ������������ ���������� ����������
							if (tables.lextable.table[j].lexema == LEX_ID || tables.lextable.table[j].lexema == LEX_LITERAL)
							{
								paramscount++;
								if (e.value.params.count == NULL)
									break;
								IT::IDDATATYPE ctype = tables.idtable.table[tables.lextable.table[j].idxTI].iddatatype;
								if (ctype != e.value.params.types[paramscount - 1])
								{
									// ������������ ����� ������������ ����������
									throw ERROR_THROW_IN(309, tables.lextable.table[i].sn, 0);
									sem_ok = false;
									break;
								}
							}
							if (j == tables.lextable.size)
								break;
						}
						if (paramscount != e.value.params.count)
						{
							// ���������� ������������ � ����������� ���������� �� ���������
							throw ERROR_THROW_IN(308, tables.lextable.table[i].sn, 0);
							sem_ok = false;
						}
						if (paramscount > 3)
						{
							// ������� ����� ���������� � ������
							throw ERROR_THROW_IN(307, tables.lextable.table[i].sn, 0);
							sem_ok = false;
						}
					}
				}
				break;
			}
			case LEX_MORE:	case LEX_LESS: case LEX_EQUALS:   case LEX_NOTEQUALS:
			{
				// ����� � ������ ������� - �������� ���
				bool flag = true;
				if (i > 1 && tables.lextable.table[i - 1].idxTI != NULLIDX_TI)
				{
					if (tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype != IT::IDDATATYPE::NUM)
						flag = false;
				}
				if (tables.lextable.table[i + 1].idxTI != NULLIDX_TI)
				{
					if (tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype != IT::IDDATATYPE::NUM)
						flag = false;
				}
				if (!flag)
				{
					// ������ ��� ����������� �� � �������
					throw ERROR_THROW_IN(317, tables.lextable.table[i].sn, 0);
					sem_ok = false;
				}
				break;
			}
			} 
		} 

		return sem_ok;
	}
};