module Sevenseq2(data, seq);
	input [3:0]data;
	output [15:0]seq;
	reg [15:0]seq;
	
	always @(data)
	begin
		case(data)
				4'b0000: seq <= 16'b1100000011000000;
				4'b0001: seq <= 16'b1100000011111001;
				4'b0010: seq <= 16'b1100000010100100;
				4'b0011: seq <= 16'b1100000010110000;
				4'b0100: seq <= 16'b1100000010011001;
				4'b0101: seq <= 16'b1100000010010010;
				4'b0110: seq <= 16'b1100000010000010;
				4'b0111: seq <= 16'b1100000011111000;
				4'b1000: seq <= 16'b1100000010000000;
				4'b1001: seq <= 16'b1100000010010000;
				
				4'b1010: seq <= 16'b1111100111000000;
				4'b1011: seq <= 16'b1111100111111001;
				4'b1100: seq <= 16'b1111100110100100;
				4'b1101: seq <= 16'b1111100110110000;
				4'b1110: seq <= 16'b1111100110011001;
				4'b1111: seq <= 16'b1111100110010010;
		endcase
	end
	
endmodule


module compare(a,b,LEDC);
	input [3:0]a,b;
	output[9:0]LEDC;
	reg [9:0]LEDC;
	always @(*)
	begin
	if(a==b)
		LEDC = 8'b11111111; 
	else LEDC = 8'b00000000; 
	end
endmodule 

	
module FinalProject(player1,player2,player1_output,player2_output,LEDC);
	input [3:0]player1,player2;
	output [15:0]player1_output,player2_output;
	output[9:0]LEDC;
		
	Sevenseq2 S2(player2,player2_output);
	Sevenseq2 S1(player1,player1_output);
	
	compare C(player1,player2,LEDC);
	
endmodule
	