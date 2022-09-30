#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;


int his[17][3] = { 0 };///紀錄history //000
int state[17][8] = { 0 };//紀錄狀態// 0->SN // 1->WN // 2->WT // 3->ST
int which_address = 0;//紀錄當前output_address為第幾個
int a;//a & b是處理entry用的// a 表 entry數量
vector<string> output_entry[17];//要輸出的entries
vector<string>output_address;//按照模擬的順序紀錄address
vector<string>address;//存讀到的address
vector<string>data_inst;//存讀到的instruction//addi or beq
vector<string>data_reg;//存讀到的registor//R1 R2....
vector<int>data_entry;//從0開始數  存第幾行所對應到第幾個entry

void begin_output(int a)//初始化output_entry
{
	string buffer = { " ( 000, SN SN SN SN SN SN SN SN )" };
	for (int i = 0; i < a; i++)
	{
		output_entry[i].push_back(buffer);
	}
}


void solution(int ent, char u)// ent 表第幾個entry  // u 表outcome
{
	vector<string> temp;//目前所要的entry

	//output address
	cout << output_address[which_address] << "      ";
	cout << "entry: " << ent << endl;

	which_address++;

	int num = his[ent][2] * 1 + his[ent][1] * 2 + his[ent][0] * 4;//2進位轉10進位
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
		if (state[ent][num] == 0 || state[ent][num] == 1 || state[ent][num] == 2)state[ent][num]++;//ST狀態遇到T不變
		his[ent][2] = 1;//update history
	}
	else //u==N
	{
		if (state[ent][num] == 3 || state[ent][num] == 1 || state[ent][num] == 2)state[ent][num]--;//SN狀態遇到N不變
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
	cin >> a;// a 表 entry數量
	while (a % 2 == 1)cin >> a;//entry因為是看pc裡的bit所以不會有奇數個
	cout << endl;
	begin_output(a);//初始化所有entry

	int n = 0, m = -1, loop_num, back_num;// 每行讀進來的分三個部分存: address/inst/reg 用n做紀錄 
										  // m用來算Loop從第幾行開始以及到第幾行結束

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
			if (str == "R0,R0,Loop")back_num = m;//Loop結束的位置
			if (str == "Loop:")loop_num = m;//Loop開始的位置
			else if (str == "End:")break;//End不理她
			else
			{
				if (n == 0)//n=0時為address
				{
					address.push_back(str), m++, n++;

					//給予對應的第幾個entry
					if (b != a - 1)data_entry.push_back(b), b++;
					else data_entry.push_back(b), b = 0;
				}
				else if (n == 1)data_inst.push_back(str), n++;//n==1 instruction
				else data_reg.push_back(str), n = 0;//n==2 registor
			}
		}
	}

	int simulation[10] = { 0 };//模擬執行的主角 存的為對應的reg的值//ex:  R2=simulation[2]
	for (int i = 0; i < loop_num; i++)//處理Loop以前
	{
		int negtive = 1;//處理負值
		int fir, sec, value;// addi 的三個主角 // ex:addi R2,R0,4  // fir=R2 sec=R0 value=4
		int times = 1;//第一次讀到數字就是fir依序為sec value

		output_address.push_back(address[i]);//對應的address & entry
		callfuc(i, 'N'); // 這部分沒有任何branch//均為 N

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
		simulation[fir] = simulation[sec] + value;//因為只會有addi
	}

	int beq_1, beq_2;//LOOP的branch條件 //當 beq_1==beq_2 -> 跳
	int middle_addi[10][2] = { 0 };//LOOP裡的addi
	int value[10]{ 0 };//LOOP裡的addi的value
	for (int i = loop_num; i < back_num; i++)//處理Loop
	{
		int times = 1;//作用跟上面的差不多
		int negtive = 1;

		for (auto& k : data_reg[i])
		{
			if (k == '-')negtive = -1;
			if (data_inst[i] == "beq" && i != back_num)//紀錄LOOP所要用到的兩個registor
			{
				if (isdigit(k))
				{
					if (times == 1) beq_1 = atoi(&k), times++;
					else beq_2 = atoi(&k), times = 1;
				}
			}
			else
			{
				if (isdigit(k))//LOOP裡執行的addi們
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

	//模擬LOOP
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