// KVS-2021.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "PolishNotation.h"
#include "Generator.h"
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	char lexerror[] = "Лексический анализ завершен с ошибками";
	char synterror[] = "Синтаксический анализ завершен с ошибками";
	char semerror[] = "Обнаружены семантические ошибки";
	char polisherror[] = "Ошибка при попытке преобразования выражения";
	char lexgood[] = "Лексический анализ завершен без ошибок";
	char syntgood[] = "Синтаксический анализ завершен без ошибок";
	char semgood[] = "Семантический анализ завершен без ошибок";
	char polishgood[] = "Преобразование выражений завершено без ошибок";
	char allgood[] = "Программа успешно завершена!";
	setlocale(LC_ALL, "Russian");
	Log::LOG log;

	try
	{

		Parm::PARM parm = Parm::getparm(argc, argv);                            //получить параметры
		log = Log::getstream(parm.log);
		Log::writeLog(log);														//написать заголовок журнала
		Log::writeParm(log, parm);												//записать в журнал параметры
		In::IN in = In::getin(parm.in, log.stream);


		Log::writeIn(log.stream, in);

		in.words = In::getWordsTable(log.stream, in.text, in.code, in.size);	//разобрать на слова

		Lexer::LEX tables;
		bool lex_ok = Lexer::analyze(tables, in, log, parm);					//выполнить лексический анализ
		LT::writeLexTable(log.stream, tables.lextable);							//записать в журнал таблицы лексем и идентификаторов 
		IT::writeIdTable(log.stream, tables.idtable);							//а также соответствие токенов и лексем
		LT::writeLexemsOnLines(log.stream, tables.lextable);
		if (!lex_ok)
		{
			Log::writeLine(log.stream, lexerror, "");
			return 0;
		}
		MFST_TRACE_START(log.stream);
		MFST::Mfst mfst(tables, GRB::getGreibach());							//выполнить синтаксический анализ
		bool synt_ok = mfst.start(log);
		mfst.savededucation();
		mfst.printrules(log);													//вывести дерево разбора
		if (!synt_ok)
		{
			Log::writeLine(log.stream, synterror, "");
			return 0;
		}
		else Log::writeLine(&std::cout, syntgood, "");
		bool sem_ok = Semantic::semanticsCheck(tables, log);					//выполнить семантический анализ
		if (!sem_ok)
		{
			Log::writeLine(log.stream, semerror, "");
			return 0;
		}
		else Log::writeLine(&std::cout, semgood, "");
		bool polish_ok = Polish::PolishNotation(tables, log);					//выполнить преобразование выражений в ПОЛИЗ
		if (!polish_ok)
		{
			Log::writeLine(log.stream, polisherror, "");
			return 0;
		}
		else Log::writeLine(&std::cout, polishgood, "");
		LT::writeLexemsOnLines(log.stream, tables.lextable);					//а также соответствие токенов и лексем
		Gener::CodeGeneration(tables, parm, log);								//выполнить генерацию кода
		Log::writeLine(log.stream, allgood, "");									//итог работы программы
		Log::writeLine(&std::cout, allgood, "");

		Log::Close(log);													    //закрыть журнал
	}
	catch (Error::ERROR e)
	{
		cout << e.message << endl << "stroka "<< e.inext.line;
	}
}
