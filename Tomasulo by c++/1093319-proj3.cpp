#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<queue>
#include<stack>
using namespace std;

typedef struct A//data
{
	string Reg;
	int Fa;
	int Fb;
	int Fc;
}A;

typedef struct B//rs
{
	char Operator;
	int b;
	int c;
	bool ready_all = false;//true means ready
	bool ready_b = false;
	bool ready_c = false;
	bool empty = true;//true means empty
}B;

typedef struct C//buffer
{
	int rs;
	int b;
	char opert;
	int c;
	int cycl;//����run�X��cycle��i�Hwr
}C;

int RF[6] = { -1,0,2,4,6,8 };//�Ĥ@��-1���n��
vector<int> RAT[5] = {};//�l�׫��C���u�঳�@�ӼƦr
//vector<stack<int>>RAT[5];
B RS_add[3] = {};
B RS_mul[2] = {};
int cycle = 0;
//const int add_cycle = 2, mul_cycle = 10, div_cycle = 40;
bool unit_add = true;//ALU unit//true means empty
bool unit_mul = true;
C buffer_add = {};
C buffer_mul = {};
bool out = false;//�M�woutput�P�_
int RS_posa, RS_posm;//�����{�b���bunit���檺RS��RS�X
//����j�I�s�ɦh��
bool checkdis_a = false;//true�ɪ�ܦA���I�s ���`�@�u���L���@��
bool checkdis_m = false;


bool issue(A inst)
{
	bool find_empty = false;
	//int whrs;//where is rs

	//RS���Ŧ�~�i�H�i
	if (inst.Reg == "ADDI" || inst.Reg == "ADD" || inst.Reg == "SUB")
	{
		for (int i = 0; i < 3; i++)//�q�Ĥ@�Ӷ}�l�䦳�Ū�RS
		{
			if (RS_add[i].empty)// RSi ----> ex:i=0 ,��RS1//�n�[1
			{
				RS_add[i].empty = false;//��s��������
				find_empty = true;//RS����l�i�Hissue
				//whrs = i + 1;

				// operator: +
				if (inst.Reg == "ADDI" || inst.Reg == "ADD")RS_add[i].Operator = '+';
				// operator: -
				else RS_add[i].Operator = '-';

				//check RAT
				/*******************check inst.b �bRAT�����L�F��**************************/
				if (RAT[inst.Fb - 1].empty())//RAT�S�F�� 
				{
					RS_add[i].b = RF[inst.Fb];//�hFx�Ȭ�RF�̪���
					//RAT[inst.Fa - 1].push_back(i + 1);//RAT����������RSi//inst.Fa��ܬ�Fx//ex: inst.Fa=1  ��F1
					RS_add[i].ready_b = true;//RS.b is already 
				}
				else//RAT������//�n���ݧO�Hwrite back
				{
					//RS_add is not ready
					RS_add[i].ready_b = false;
					RS_add[i].b = RAT[inst.Fb - 1][0];//����RS�s��
					//RAT[inst.Fa - 1].push_back(i + 1);//RAT����������RSi//inst.Fa��ܬ�Fx//ex: inst.Fa=1  ��F1
				}

				/*******************check inst.c �bRAT�����L�F��**************************/
				if (inst.Reg == "ADDI")
				{
					RS_add[i].c = inst.Fc;//addi , so inst.Fc is a number//ready is already true
					RS_add[i].ready_c = true;
				}
				else//add && sub
				{
					if (RAT[inst.Fc - 1].empty())//RAT�S�F�� 
					{
						RS_add[i].c = RF[inst.Fc];//�hFx�Ȭ�RF�̪���
						RS_add[i].ready_c = true;
					}
					else//RAT������//�n���ݧO�Hwrite back
					{
						//RS_add is not ready
						RS_add[i].ready_c = false;//�N��RS.b is true���o��RS.c�٬Ofalse
						RS_add[i].c = RAT[inst.Fc - 1][0];//����RS�s��
					}
				}

				/************inst.a �bRAT�����LRS******************************************/
				if (!RAT[inst.Fa - 1].empty())RAT[inst.Fa - 1].clear();//�YRAT�w��RS�h���M���b�s�W
				RAT[inst.Fa - 1].push_back(i + 1);//RAT����������RSi//inst.Fa��ܬ�Fx//ex: inst.Fa=1  ��F1

				/***��s����ready sate***/
				if (RS_add[i].ready_b && RS_add[i].ready_c)RS_add[i].ready_all = true;//already for dispatch
				break;
			}
		}
	}
	else//MUL DIV ----> RS�s��+3
	{
		for (int i = 0; i < 2; i++)//�q�Ĥ@�Ӷ}�l�䦳�Ū�RS
		{
			if (RS_mul[i].empty)
			{
				RS_mul[i].empty = false;//��s��������
				find_empty = true;//RS����l�i�Hissue
				//whrs = i + 1;

				// operator: +
				if (inst.Reg == "MUL")RS_mul[i].Operator = '*';
				// operator: /
				else RS_mul[i].Operator = '/';

				//check RAT
				/*******************check inst.b �bRAT�����L�F��**************************/
				if (RAT[inst.Fb - 1].empty())//RAT�S�F�� 
				{
					RS_mul[i].b = RF[inst.Fb];//�hFx�Ȭ�RF�̪���
					RS_mul[i].ready_b = true;//RS.b is already 
				}
				else//RAT������//�n���ݧO�Hwrite back
				{
					RS_mul[i].ready_b = false;
					RS_mul[i].b = RAT[inst.Fb - 1][0];//����RS�s��
				}

				/*******************check inst.c �bRAT�����L�F��**************************/
				if (RAT[inst.Fc - 1].empty())//RAT�S�F�� 
				{
					RS_mul[i].c = RF[inst.Fc];//�hFx�Ȭ�RF�̪���
					RS_mul[i].ready_c = true;
				}
				else//RAT������//�n���ݧO�Hwrite back
				{
					RS_mul[i].ready_c = false;//�N��RS.b is true���o��RS.c�٬Ofalse
					RS_mul[i].c = RAT[inst.Fc - 1][0];//����RS�s��
				}

				/************inst.a �bRAT�����LRS******************************************/
				if (!RAT[inst.Fa - 1].empty())RAT[inst.Fa - 1].clear();//�YRAT�w��RS�h���M���b�s�W
				RAT[inst.Fa - 1].push_back(i + 4);//RAT����������RSi//inst.Fa��ܬ�Fx//ex: inst.Fa=1  ��F1

				/***��s����ready sate***/
				if (RS_mul[i].ready_b && RS_mul[i].ready_c)RS_mul[i].ready_all = true;//already for dispatch
				break;
			}
		}
	}

	return find_empty;
}

void write_back(int rs, int result)//RS=rs+1
{
	//capture
	for (auto& u : RS_add)//RS���������Ƶ���
	{
		if (u.ready_all)continue;
		else if (u.empty)continue;

		if (!u.ready_b && u.b == rs + 1)
		{
			u.b = result;
			u.ready_b = true;//get value ----> become ready
		}
		if (!u.ready_c && u.c == rs + 1)
		{
			u.c = result;
			u.ready_c = true;//get value ----> become ready
		}
		if (u.ready_b && u.ready_c)
		{
			u.ready_all = true;
			checkdis_a = true;
		}
	}

	for (auto& u : RS_mul)//RS���������Ƶ���
	{
		if (u.ready_all)continue;
		else if (u.empty)continue;
		if (!u.ready_b && u.b == rs + 1)
		{
			u.b = result;
			u.ready_b = true;//get value ----> become ready
		}
		if (!u.ready_c && u.c == rs + 1)
		{
			u.c = result;
			u.ready_c = true;//get value ----> become ready
		}
		if (u.ready_b && u.ready_c)
		{
			u.ready_all = true;
			checkdis_m = true;
		}
	}

	//Update value of RS in RAT
	bool wr_RF = false;
	for (int i = 0; i < 5; i++)
	{
		if (!RAT[i].empty() && RAT[i][0] == rs + 1)
		{
			RAT[i].clear();//����RAT
			wr_RF = true;//update value in RF
			if (wr_RF)RF[i + 1] = result;
		}
	}

}

void dispatch()
{
	if (unit_add && checkdis_a)checkdis_a = false;
	else if (unit_add)//add unit is empty
	{
		for (int i = 0; i < 3; i++)
		{
			if (RS_add[i].ready_all)//��@��ready�i�Jbuffer
			{
				out = true;
				RS_posa = i;
				buffer_add.rs = i;
				unit_add = false;
				buffer_add.opert = RS_add[i].Operator;
				buffer_add.b = RS_add[i].b;
				buffer_add.c = RS_add[i].c;
				buffer_add.cycl = 2 - 1;//���]ADD ADDi SUB ���n��2 cycle
				break;
			}
		}
	}
	else
	{
		if (buffer_add.cycl == 0)//�i�Hwr
		{
			out = true;
			int result;
			if (buffer_add.opert == '+')result = buffer_add.b + buffer_add.c;//count the result
			else result = buffer_add.b - buffer_add.c;

			RS_add[RS_posa] = {};//RS����
			write_back(buffer_add.rs, result);//wr to that rs
			buffer_add = {};//buffer become empty
			unit_add = true;//become empty
			checkdis_m = true;
			dispatch();//unit�Ŷ����X �ҥH�i�H��@��ready���@dispatch//�����a�^�|�h�ˤ@��1
		}
		else buffer_add.cycl--;
	}

	/**********************************************************/
	if (checkdis_m)
	{
		checkdis_m = false;
		return;
	}
	else if (unit_mul && checkdis_m)checkdis_m = false;
	else if (unit_mul)//mul unit is empty
	{
		for (int i = 0; i < 2; i++)
		{
			if (RS_mul[i].ready_all)//��@��ready�i�Jbuffer
			{
				out = true;
				RS_posm = i;
				buffer_mul.rs = i;
				unit_mul = false;
				buffer_mul.opert = RS_mul[i].Operator;
				buffer_mul.b = RS_mul[i].b;
				buffer_mul.c = RS_mul[i].c;
				//���]MUL DIV�ӧO�ݭn10/40��cycle
				if (buffer_mul.opert == '/')buffer_mul.cycl = 40 - 1;
				else buffer_mul.cycl = 10 - 1;
				break;
			}
		}
	}
	else
	{
		if (buffer_mul.cycl == 0)//�i�Hwr
		{
			out = true;
			int result;
			if (buffer_mul.opert == '/')result = buffer_mul.b / buffer_mul.c;
			else result = buffer_mul.b * buffer_mul.c;

			RS_mul[RS_posm] = {};
			write_back(buffer_mul.rs + 3, result);//wr to that rs
			buffer_mul = {};//buffer become empty
			unit_mul = true;//become empty
			//checkdis_a = true;
			dispatch();//unit�Ŷ����X �ҥH�i�H��@��ready���@dispatch
		}
		else buffer_mul.cycl--;
	}
}

void cout_RS(B n)
{
	if (n.ready_all)
	{
		cout << " |    " << n.b << " |    " << n.c << " |" << endl;
		return;
	}
	if (!n.ready_b)cout << " |  RS" << n.b;
	else cout << " |    " << n.b;
	if (!n.ready_c)cout << " |  RS" << n.c << " |" << endl;
	else cout << " |    " << n.c << " |" << endl;
}
void output()//���ܤƤ~�noutput
{
	if (!out)return;//�S�ܤ�
	else
	{
		cout << "Cycle: " << cycle << endl;
		//RF
		cout << "    _ RF __" << endl << "F1 |   " << RF[1] << "  |" << endl << "F2 |   " << RF[2] << "  |" << endl
			<< "F3 |   " << RF[3] << "  |" << endl << "F4 |   " << RF[4] << "  |" << endl << "F5 |   " << RF[5] << "  |" << endl
			<< "    --------" << endl << endl;
		//RAT
		cout << "   _ RAT __" << endl;
		if (RAT[0].empty())cout << "F1 |" << "    |" << endl;
		else cout << "F1 |" << "RS" << RAT[0][0] << " |" << endl;

		if (RAT[1].empty())cout << "F2 |" << "    |" << endl;
		else cout << "F2 |" << "RS" << RAT[1][0] << " |" << endl;

		if (RAT[2].empty())cout << "F3 |" << "    |" << endl;
		else cout << "F3 |" << "RS" << RAT[2][0] << " |" << endl;

		if (RAT[3].empty())cout << "F4 |" << "    |" << endl;
		else cout << "F4 |" << "RS" << RAT[3][0] << " |" << endl;

		if (RAT[4].empty())cout << "F5 |" << "    |" << endl;
		else cout << "F5 |" << "RS" << RAT[4][0] << " |" << endl;

		//RS
		cout << "   -------" << endl << endl << "    _ RS _________________" << endl;
		if (RS_add[0].empty)cout << "RS1 |      |      |      |" << endl;
		else cout << "RS1 |    " << RS_add[0].Operator, cout_RS(RS_add[0]);

		if (RS_add[1].empty)cout << "RS2 |      |      |      |" << endl;
		else cout << "RS2 |    " << RS_add[1].Operator, cout_RS(RS_add[1]);

		if (RS_add[2].empty)cout << "RS3 |      |      |      |" << endl;
		else cout << "RS3 |    " << RS_add[2].Operator, cout_RS(RS_add[2]);

		cout << "     ---------------------" << endl;
		if (unit_add)cout << "BUFFER: empty" << endl << endl << endl;
		else cout << "BUFFER:  (RS" << buffer_add.rs + 1 << ") " << buffer_add.b << " " << buffer_add.opert << " " << buffer_add.c << endl << endl << endl;

		if (RS_mul[0].empty)cout << "RS4 |      |      |      |" << endl;
		else cout << "RS4 |    " << RS_mul[0].Operator, cout_RS(RS_mul[0]);

		if (RS_mul[1].empty)cout << "RS5 |      |      |      |" << endl;
		else cout << "RS5 |    " << RS_mul[1].Operator, cout_RS(RS_mul[1]);

		cout << "     -----------------------" << endl;
		if (unit_mul)cout << "BUFFER: empty" << endl << endl << endl;
		else cout << "BUFFER:  (RS" << buffer_mul.rs + 1 + 3 << ") " << buffer_mul.b << " " << buffer_mul.opert << " " << buffer_mul.c << endl <<
			endl << "************************************" << endl << endl;
	}
}

void Tomasulo(queue<A>& data)
{
	
	bool  first = true;//�Ĥ@�����ܦp�G�i�Hdispatch�n�b�U�@��cycle�~�i�H��

	while (!data.empty())
	{
		cycle++;

		if (!first)dispatch();
		else first = false;
		A u = data.front();
		//cout << u.Reg << " " << u.Fa << "," << u.Fb << "," << u.Fc << endl;
		if (issue(u))
		{
			data.pop();//issue sucessful
			out = true;//out���ܤƴN�noutput
		}

		output();
		out = false;
	}

	while (!unit_add || !unit_mul)//unit�٦��F��ҥH�n�~���U�h
	{
		cycle++;
		dispatch();
		output();
		out = false;
	}
}



int main()
{
	string str;
	queue<A> Data;

	ifstream infile("input.txt", ios::in);
	if (!infile.is_open())
	{
		cout << "file can not be open" << endl;
		return 0;
	}
	else
	{
		A temp;
		int  count = 1;

		while (infile >> str)
		{
			//cout << str << "   ";
			if (count == 1)
			{
				temp.Reg = str;
				count++;
			}
			else if (count == 2)
			{
				for (int i = 0; i < str.size(); i++)
				{
					if (isdigit(str[i]))
					{
						if (isdigit(str[i + 1]))
						{
							temp.Fa = atoi(&str[i]);
							i++;//���Ʀa�n���L
						}
						else
							temp.Fa = atoi(&str[i]);
					}
				}
				count++;
			}
			else if (count == 3)
			{
				for (int i = 0; i < str.size(); i++)
				{
					if (isdigit(str[i]))
					{
						if (isdigit(str[i + 1]))
						{
							temp.Fb = atoi(&str[i]);
							i++;//���Ʀa�n���L
						}
						else
							temp.Fb = atoi(&str[i]);
					}
				}
				count++;
			}
			else//count==4
			{
				for (int i = 0; i < str.size(); i++)
				{
					if (isdigit(str[i]))
					{
						if (isdigit(str[i + 1]))
						{
							temp.Fc = atoi(&str[i]);
							i++;//���Ʀa�n���L
						}
						else
							temp.Fc = atoi(&str[i]);
					}
				}
				count = 1;
				Data.push(temp);
				temp = {};//��l��
			}
		}
	}

	Tomasulo(Data);

	infile.close();
	return 0;
}