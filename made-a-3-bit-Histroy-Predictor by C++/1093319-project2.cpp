#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;


int his[17][3] = { 0 };///����history //000
int state[17][8] = { 0 };//�������A// 0->SN // 1->WN // 2->WT // 3->ST
int which_address = 0;//������eoutput_address���ĴX��
int a;//a & b�O�B�zentry�Ϊ�// a �� entry�ƶq
vector<string> output_entry[17];//�n��X��entries
vector<string>output_address;//���Ӽ��������Ǭ���address
vector<string>address;//�sŪ�쪺address
vector<string>data_inst;//�sŪ�쪺instruction//addi or beq
vector<string>data_reg;//�sŪ�쪺registor//R1 R2....
vector<int>data_entry;//�q0�}�l��  �s�ĴX��ҹ�����ĴX��entry

void begin_output(int a)//��l��output_entry
{
	string buffer = { " ( 000, SN SN SN SN SN SN SN SN )" };
	for (int i = 0; i < a; i++)
	{
		output_entry[i].push_back(buffer);
	}
}


void solution(int ent, char u)// ent ��ĴX��entry  // u ��outcome
{
	vector<string> temp;//�ثe�ҭn��entry

	//output address
	cout << output_address[which_address] << "      ";
	cout << "entry: " << ent << endl;

	which_address++;

	int num = his[ent][2] * 1 + his[ent][1] * 2 + his[ent][0] * 4;//2�i����10�i��
	char pre;//prediction

	if (state[ent][num] == 0 || state[ent][num] == 1)pre = 'N';//do  predict
	else pre = 'T';


	for (int i = 0; i < a; i++)//output entries
	{
		for (int j = 0; j < output_entry[i].size(); j++)
		{
			cout << output_entry[i][j];
		}
		cout << endl;
	}

	//output outcome & prediction
	cout << "Outcome: " << u << "   " << " Prediction: " << pre << " >> ";

	if (u == pre)cout << "correct";//check predict susses, or not
	else cout << "miss";

	//update state & history
	his[ent][0] = his[ent][1];
	his[ent][1] = his[ent][2];

	if (u == 'T')
	{
		if (state[ent][num] == 0 || state[ent][num] == 1 || state[ent][num] == 2)state[ent][num]++;//ST���A�J��T����
		his[ent][2] = 1;//update history
	}
	else //u==N
	{
		if (state[ent][num] == 3 || state[ent][num] == 1 || state[ent][num] == 2)state[ent][num]--;//SN���A�J��N����
		his[ent][2] = 0;
	}

	//update history in answer vector
	temp.push_back(" ( ");
	for (auto& n : his[ent])
	{
		string m(to_string(n));
		temp.push_back(m);
	}
	temp.push_back(", ");

	for (auto& m : state[ent])//update states
	{
		if (m == 0) temp.push_back("SN ");
		else if (m == 1)temp.push_back("WN ");
		else if (m == 2)temp.push_back("WT ");
		else temp.push_back("ST ");
	}

	temp.push_back(")  ");
	output_entry[ent] = temp;//update ent'th row of output_entry

	cout << endl << endl << endl;

}

void callfuc(int num, char pre)
{
	for (auto& n : data_inst[num])cout << n;
	cout << " ";
	for (auto& n : data_reg[num])cout << n;
	cout << "     ";
	solution(data_entry[num], pre);
}

int main()
{
	string str;


	int b = 0;
	cout << "entry?" << endl;
	cin >> a;// a �� entry�ƶq
	while (a % 2 == 1)cin >> a;//entry�]���O��pc�̪�bit�ҥH���|���_�ƭ�
	cout << endl;
	begin_output(a);//��l�ƩҦ�entry

	int n = 0, m = -1, loop_num, back_num;// �C��Ū�i�Ӫ����T�ӳ����s: address/inst/reg ��n������ 
										  // m�ΨӺ�Loop�q�ĴX��}�l�H�Ψ�ĴX�浲��

	ifstream infile("test2.txt", ios::in);
	if (!infile.is_open())
	{
		cout << "file can not be open" << endl;
		return 0;
	}
	else
	{
		while (infile >> str)
		{
			if (str == "R0,R0,Loop")back_num = m;//Loop��������m
			if (str == "Loop:")loop_num = m;//Loop�}�l����m
			else if (str == "End:")break;//End���z�o
			else
			{
				if (n == 0)//n=0�ɬ�address
				{
					address.push_back(str), m++, n++;

					//�����������ĴX��entry
					if (b != a - 1)data_entry.push_back(b), b++;
					else data_entry.push_back(b), b = 0;
				}
				else if (n == 1)data_inst.push_back(str), n++;//n==1 instruction
				else data_reg.push_back(str), n = 0;//n==2 registor
			}
		}
	}

	int simulation[10] = { 0 };//�������檺�D�� �s����������reg����//ex:  R2=simulation[2]
	for (int i = 0; i < loop_num; i++)//�B�zLoop�H�e
	{
		int negtive = 1;//�B�z�t��
		int fir, sec, value;// addi ���T�ӥD�� // ex:addi R2,R0,4  // fir=R2 sec=R0 value=4
		int times = 1;//�Ĥ@��Ū��Ʀr�N�Ofir�̧Ǭ�sec value

		output_address.push_back(address[i]);//������address & entry
		callfuc(i, 'N'); // �o�����S������branch//���� N

		for (auto& k : data_reg[i])
		{
			if (k == '-')negtive = -1;
			if (isdigit(k))
			{
				if (times == 1)fir = atoi(&k), times++;
				else if (times == 2) sec = atoi(&k), times++;
				else
				{
					times = 0, value = atoi(&k) * negtive;
					if (negtive == -1)negtive = 1;
				}
			}
		}
		simulation[fir] = simulation[sec] + value;//�]���u�|��addi
	}

	int beq_1, beq_2;//LOOP��branch���� //�� beq_1==beq_2 -> ��
	int middle_addi[10][2] = { 0 };//LOOP�̪�addi
	int value[10]{ 0 };//LOOP�̪�addi��value
	for (int i = loop_num; i < back_num; i++)//�B�zLoop
	{
		int times = 1;//�@�θ�W�����t���h
		int negtive = 1;

		for (auto& k : data_reg[i])
		{
			if (k == '-')negtive = -1;
			if (data_inst[i] == "beq" && i != back_num)//����LOOP�ҭn�Ψ쪺���registor
			{
				if (isdigit(k))
				{
					if (times == 1) beq_1 = atoi(&k), times++;
					else beq_2 = atoi(&k), times = 1;
				}
			}
			else
			{
				if (isdigit(k))//LOOP�̰��檺addi��
				{
					if (times == 1)middle_addi[i][0] = atoi(&k), times++;
					else if (times == 2)middle_addi[i][1] = atoi(&k), times++;
					else
					{
						times = 1;
						value[i] = negtive * atoi(&k);
						if (negtive == -1)negtive = 1;
					}
				}
			}

		}
	}

	//����LOOP
	for (int i = loop_num; simulation[beq_1] != simulation[beq_2]; i = loop_num)//Loop:	beq	R1,R2,End
	{
		output_address.push_back(address[i]);
		callfuc(i, 'N');

		i++;
		for (int j = i; j <= back_num; j++)
		{
			if (data_inst[j] == "addi")
			{
				output_address.push_back(address[j]);

				simulation[middle_addi[j][0]] = simulation[middle_addi[j][1]] + value[j];
				callfuc(j, 'N');
			}
			else
			{
				output_address.push_back(address[j]);
				callfuc(j, 'T');
			}
		}
	}

	output_address.push_back(address[loop_num]);
	callfuc(loop_num, 'T');

	infile.close();
	return 0;

}