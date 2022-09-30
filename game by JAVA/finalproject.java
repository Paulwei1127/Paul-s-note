import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import java.awt.FlowLayout;
import java.awt.Font;
public class finalproject extends JFrame implements ActionListener {
	
	private JLabel turn;
	private JLabel situation;
	private JPanel jpane;
	private JButton cancel ;
	private JButton[] button = new JButton[9];
	private JButton[] red_button = new JButton[3];
	private JButton[] blue_button = new JButton[3];	
	boolean red_turn = true;
	boolean select_board = false;
	boolean first = true;
	int now_select=0;
	int now_select_board;
	int[] record = new int[9];
	int[] red_not_yet = new int[3];
	int[] blue_not_yet = new int[3];
	
	public finalproject() {   // 建構子		
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		setBounds(250, 250, 820, 575);
        setTitle("finalproject");
         		
		jpane = new JPanel();
		setContentPane(jpane);   
        jpane.setLayout(null);  // 版面配置	
		
		turn = new JLabel("紅方回合",JLabel.CENTER);
		turn.setForeground(Color.RED);
		turn.setFont(new Font("文鼎PL中楷", Font.BOLD, 40));
		situation = new JLabel("請選擇要使用的棋子",JLabel.CENTER);	
		situation.setFont(new Font("文鼎PL中楷", Font.BOLD, 40));
		turn.setBounds(0,0,820,100);
		situation.setBounds(0,425,820,100);
		jpane.add(turn);
		jpane.add(situation);
		
		cancel= new JButton("取消選取");
		cancel.setBounds(600,425,200,100);
		cancel.addActionListener(this);
		cancel.setFont(new Font("文鼎PL中楷", Font.BOLD, 35));
		jpane.add(cancel);
		red_button[0]= new JButton("兵*"+3); 		
		blue_button[0]= new JButton("卒*"+3); 
		red_button[1]= new JButton("俥*"+2); 		
		blue_button[1]= new JButton("車*"+3); 
		red_button[2]= new JButton("帥*"+2); 		
		blue_button[2]= new JButton("將*"+2); 
		for(int i=0;i<3;i++){//紅黑按鈕版面配置
			red_not_yet[i]=2;
			blue_not_yet[i]=2;
			red_button[i].setForeground(Color.RED);
			blue_button[i].setForeground(Color.BLACK);
			red_button[i].setBounds(0,100+i*100,125,100);
			blue_button[i].setBounds(675,100+i*100,125,100);
			blue_button[i].setFont(new Font("文鼎PL中楷", Font.BOLD, 40));
			red_button[i].setFont(new Font("文鼎PL中楷", Font.BOLD, 40));
			blue_button[i].addActionListener(this);
			red_button[i].addActionListener(this);
			jpane.add(red_button[i]);
			jpane.add(blue_button[i]);
		}
		red_not_yet[0]=2;
		blue_not_yet[0]=3;
		blue_not_yet[1]=3;
		for(int i=0;i<9;i++){//棋盤按鈕配置
			record[i]=0;
			button[i] = new JButton("");    
			button[i].setBounds(250+(i/3)*100, 100+(i%3)*100, 100, 100);  
			button[i].addActionListener(this);
			button[i].setFont(new Font("文鼎PL中楷", Font.BOLD, 40));
			jpane.add(button[i]);
		}
		setVisible(true);  
	}
	public void actionPerformed(ActionEvent e){
		if(e.getSource() == cancel){
			select_board = false;
			now_select=0;
			situation.setText("請選取要使用的棋子");
			return;
		}
		for(int i=0;i<3;i++){
			if(e.getSource() == red_button[i]){
				if(red_turn){
					if(red_not_yet[i]==0)
						situation.setText("該棋已被用完");					
					else if(i==0){
						situation.setText("您選取兵");
					}else if(i==1){
						situation.setText("您選取俥");
					}else if(i==2){
						situation.setText("您選取帥");
					}
					select_board = false;
					now_select=i+1;
				}else{
					situation.setText("現在不是你的回合");
					now_select=0;
				}		
			}
			if(e.getSource() == blue_button[i]){
				if(!red_turn){
					if(blue_not_yet[i]==0)
						situation.setText("該棋已被用完");					
					else if(i==0){
						situation.setText("您選取了卒");
					}else if(i==1){
						situation.setText("您選取了車");
					}else if(i==2){
						situation.setText("您選取了將");
					}
					select_board = false;
					now_select=i*(-1)-1;
				}else{
					situation.setText("現在不是你的回合");
					now_select=0;
				}		
			}
		}
		
		if( now_select!=0 && select_board == false){
			for(int i=0;i<9;i++){
				if(first&&e.getSource() == button[4]){
					situation.setText("首回合不可下在該處");
				}
				else if(e.getSource() == button[i]){
					get(button[i],record,i);
					first=false;
				}
			}
		}else{
			for(int i=0;i<9;i++){
				if(e.getSource() == button[i]&&((red_turn&&record[i]>0)||(!red_turn&&record[i]<0))){
					now_select=record[i];
					situation.setText("你選取了場上的 "+getname(now_select));
					now_select_board= i;
					select_board = true;
				}else if(now_select==0&&e.getSource() == button[i]&&(record[i]==0||(red_turn&&record[i]<0)||(!red_turn&&record[i]>0)))
					situation.setText("請做出有效的選取");
			}
		}
		if(select_board){
			for(int i=0;i<now_select_board;i++){
				if(e.getSource() == button[i]){
					eat(button,now_select_board,i);
				}
			}
			for(int i=now_select_board+1;i<9;i++){
				if(e.getSource() == button[i]){
					eat(button,now_select_board,i);
				}
			}
		}
	}
	public void eat(JButton A[],int eat,int eated){
		if(record[eat]*record[eated]<0&&(Math.abs(record[eat])>Math.abs(record[eated])&& //確認該棋可以吃
			(eat+1==eated||eat-1==eated||eat-3==eated||eat+3==eated))){//確認在四周
			record[eated] = record[eat];
			record[eat] = 0;
			button[eated].setText(getname(record[eated]));
			button[eated].setForeground(red_turn?Color.RED:Color.BLACK);
			button[eat].setText("");
			turn_change(red_turn);
			check();
		}
		else
			situation.setText("不可下在該處");
	}
	public void turn_change(boolean r_turn){
		if(r_turn)
			turn.setText("黑方回合");				
		else
			turn.setText("紅方回合");	
		turn.setForeground(r_turn?Color.BLACK:Color.RED);	
		situation.setText("請選取要使用的棋子");
		red_turn=!red_turn;
		now_select=0;
	}
	public void get(JButton A,int record[],int i){
		if((red_turn&&red_not_yet[now_select-1]==0)||(!red_turn&&blue_not_yet[now_select*-1-1]==0)){
			situation.setText("該棋已被用完");
			now_select=0;
		}
		else if(now_select*record[i]<=0&&now_select!=0&&(Math.abs(now_select)>Math.abs(record[i]))){//成功下棋	或 蓋棋		
			A.setText(getname(now_select));
			A.setForeground(red_turn?Color.RED:Color.BLACK);
			record[i] = now_select;					
			switch (now_select) {
				case 3 :
					red_not_yet[now_select-1]--;
					red_button[now_select-1].setText("帥*"+red_not_yet[now_select-1]);
					if(red_not_yet[now_select-1]==0)
						red_button[now_select-1].setForeground(Color.GRAY);
					break;
				case 2 :
					red_not_yet[now_select-1]--;
					red_button[now_select-1].setText("俥*"+red_not_yet[now_select-1]);
					if(red_not_yet[now_select-1]==0)
						red_button[now_select-1].setForeground(Color.GRAY);
					break;
				case 1 :
					red_not_yet[now_select-1]--;		
					red_button[now_select-1].setText("兵*"+red_not_yet[now_select-1]);
					if(red_not_yet[now_select-1]==0)
						red_button[now_select-1].setForeground(Color.GRAY);
					break;
				case -3 :
					blue_not_yet[now_select*-1-1]--;
					blue_button[now_select*-1-1].setText("將*"+blue_not_yet[now_select*-1-1]);
					if(blue_not_yet[now_select*-1-1]==0)
						blue_button[now_select*-1-1].setForeground(Color.GRAY);
					break;					
				case -2 :
					blue_not_yet[now_select*-1-1]--;
					blue_button[now_select*-1-1].setText("車*"+blue_not_yet[now_select*-1-1]);
					if(blue_not_yet[now_select*-1-1]==0)
						blue_button[now_select*-1-1].setForeground(Color.GRAY);
					break;
				case -1 :
					blue_not_yet[now_select*-1-1]--;
					blue_button[now_select*-1-1].setText("卒*"+blue_not_yet[now_select*-1-1]);
					if(blue_not_yet[now_select*-1-1]==0)
						blue_button[now_select*-1-1].setForeground(Color.GRAY);
					break;
			}
			turn_change(red_turn);
			check();
		}else
			situation.setText("不可下在該處");
	}
	public String getname(int now_select){
		switch (now_select) {
            case 3 :
				return "帥";
            case 2 :
                return "俥";
            case 1 :
                return "兵";	
				
            case -3 :
                return "將";			
            case -2 :
                return "車";

			case -1 :
                return "卒";
		}
		return "BUG";
	}
	public void check(){
		// 黑方獲勝判例
		String s1 = "X";
		String s2 = "O";
		if(
			(record[0]<0) &&
		    (record[1]<0) &&
			(record[2]<0) 
			){
			blackWins(0,1,2);
		}

		if(
			(record[3]<0) &&
		    (record[4]<0) &&
			(record[5]<0) 
			){
			blackWins(3,4,5);
		}

		if(
			(record[6]<0) &&
		    (record[7]<0) &&
			(record[8]<0) 
			){
			blackWins(6,7,8);
		}

		if(
			(record[0]<0) &&
		    (record[3]<0) &&
			(record[6]<0) 
			){
			blackWins(0,3,6);
		}

		if(
			(record[1]<0) &&
		    (record[4]<0) &&
			(record[7]<0) 
			){
			blackWins(1,4,7);
		}

		if(
			(record[2]<0) &&
		    (record[5]<0) &&
			(record[8]<0) 
			){
			blackWins(2,5,8);
		}

		if(
			(record[0]<0) &&
		    (record[4]<0) &&
			(record[8]<0) 
			){
			blackWins(0,4,8);
		}

		if(
			(record[2]<0) &&
		    (record[4]<0) &&
			(record[6]<0) 
			){
			blackWins(2,4,6);
		}

       // 紅方獲勝判例
		if(
			(record[0]>0) &&
		    (record[1]>0) &&
			(record[2]>0) 
			){
			redWins(0,1,2);
		}

		if(
			(record[3]>0) &&
		    (record[4]>0) &&
			(record[5]>0) 
			){
			redWins(3,4,5);
		}

		if(
			(record[6]>0) &&
		    (record[7]>0) &&
			(record[8]>0) 
			){
			redWins(6,7,8);
		}

		if(
			(record[0]>0) &&
		    (record[3]>0) &&
			(record[6]>0) 
			){
			redWins(0,3,6);
		}

		if(
			(record[1]>0) &&
		    (record[4]>0) &&
			(record[7]>0) 
			){
			redWins(1,4,7);
		}

		if(
			(record[2]>0) &&
		    (record[5]>0) &&
			(record[8]>0) 
			){
			redWins(2,5,8);
		}

		if(
			(record[0]>0) &&
		    (record[4]>0) &&
			(record[8]>0) 
			){
			redWins(0,4,8);
		}

		if(
			(record[2]>0) &&
		    (record[4]>0) &&
			(record[6]>0) 
			){
			redWins(2,4,6);
		}
	}

	public void blackWins(int a,int b,int c){
		button[a].setBackground(Color.GREEN);
		button[b].setBackground(Color.GREEN);
		button[c].setBackground(Color.GREEN);
	
		for(int i =0;i<9;i++){
			button[i].setEnabled(false);
		}
		for(int i =0;i<3;i++){
			blue_button[i].setEnabled(false);
			red_button[i].setEnabled(false);
		}
		situation.setText("!!恭喜 黑方 勝利!!");
		turn.setText("遊戲結束");
		situation.setForeground(Color.RED);
		turn.setForeground(Color.RED);
		cancel.setEnabled(false);
	}

	public void redWins(int a,int b,int c){
		button[a].setBackground(Color.GREEN);
		button[b].setBackground(Color.GREEN);
		button[c].setBackground(Color.GREEN);

		for(int i =0;i<9;i++){
			button[i].setEnabled(false);
		}
		for(int i =0;i<3;i++){
			blue_button[i].setEnabled(false);
			red_button[i].setEnabled(false);
		}
		cancel.setEnabled(false);
		situation.setText("!!恭喜 紅方 勝利!!");
		situation.setForeground(Color.RED);
		turn.setForeground(Color.RED);
		turn.setText("遊戲結束");
	}
   // 主程式
    public static void main(String[] args) {
		finalproject app=new finalproject();
	}
}
