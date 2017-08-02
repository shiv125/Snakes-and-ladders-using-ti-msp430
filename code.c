/*
 * main.c
 *
 *  Created on: Mar 24, 2017
 *      Author: Shivdutt Sharma
 *      		Ravi Shrimal
 *      		Arik Pamnani
 */
#include <msp430.h>
#include <stdlib.h>
/**
 * rows are anode based and columns are cathode based
 * To glow r4(row 4),c4(column 4) r4 should be low and c4 should high
 */

//get position will take dice and current of player as input agruements and return updated value of player along with some operations
int getpos(int dice, int pl1);// for player 1
int getpos2(int dice, int pl1);// for player 2

//transform function will take arguements row and column of current position of player and if player reaches some snake and ladder then it'll
int transform(int row, int col);

//mask is the function which will take input arguements rows and columns and will glow the respective led's
void mask(int r, int c);//for player 1
void mask2(int r, int c); //for player 2


int inverse(int row, int col);//inverse function takes arguements row and col and return current value of that player


void main(void){
	srand(time(NULL));//give seed to random generator family
	volatile unsigned int pl1;//port 2.1 --player 1
	volatile unsigned int pl2;//port 1.1 -- player 2
	volatile unsigned int r;
	volatile unsigned int j,flag,start_pl1,start_pl2;
	/**
	 * variable flag-- is to check that a player doesn't get a chance more than once
	 * variable pl1 and pl2 are positions of player 1 and player 2 respectively
	 * r is value of random dice
	 * start_pl is a check to ensure that player can start the game after getting 1 or 6
	 */
	char counter[7] = {0b00000101, 0b11111010, 0b10100100, 0b10110000, 0b10011010, 0b10010001, 0b10000001};
	//char rows[7] = {0, 9, 14, 8, 5, 1 ,7, 2};
	//char cols[5] = {0, 13, 3, 4, 10, 6};
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	P6DIR=0b11111111; // port 6 is devoted to random dice generator

	P2DIR =0b11111111;

	//controls rows of grid 2
	P4DIR =0b11111111;
	P7DIR =0b11111111;
	P8DIR =0b11111111;

	P3DIR =0b11111111;//it controls rows of grid 1 (player 1)
	P1DIR=0b11111111;//it controls columns of grid 2 (player 2)
	P2REN=BIT1;// for player 1
	P1REN=BIT1;//for player 2

	//P2DIR &= ~BIT1;//SET AS INPUT
	P1DIR &= ~BIT1;//SET AS INPUT
	P1SEL &= ~(BIT1);
	// P2SEL &= ~(BIT1);

	P6OUT=0b000000000;//random dice
	P3OUT=0b11111111;//make all the rows 1 so that no row will glow
	P2OUT=BIT1;
	P1OUT=BIT1;
	pl1=0;//initial position of player 1
	pl2=0;//initial position of player 1
	flag=0;//player 1 will get a chance first
	start_pl1=2;
	start_pl2=2;
	while (1){
		if (flag==0){
			if (!(P2IN & BIT1)){//player 1
				r=rand()%6;
				r=r+1;
				P6OUT=counter[r];
				if (r==1 || r==6){
					start_pl1=1;
				}
				if (start_pl1==1){
					pl1 = getpos(r,pl1);
				}
				for (j=50000;j>0;j--);
				flag=1;
			}
		}
		if (flag==1){
			if (!(P1IN & BIT1)){
				r=rand()%6;
				r=r+1;
				P6OUT=counter[r];
				if (r==1 || r==6){
					start_pl2=1;
				}
				if (start_pl2==1){
					pl2 = getpos2(r,pl2);
					}
				for (j=50000;j>0;j--);
				flag=0;
			}
		}

	if (pl1==35){
		P3OUT=0b00000000;
		P2OUT=0b11111111;
		for (j=50000;j>0;j--);
		P3OUT=0b11111111;

	}

	if (pl2==35){
		P4OUT=0b00000000;
		P7OUT=0b00000000;
		P8OUT=0b00000001;
		P1OUT=0b11111111;
		for (j=50000;j>0;j--);
		P1OUT=0b00000010;
	}
	}
}//end main function

	int getpos(int dice, int pl1){
		int a;
		volatile unsigned int i,j,row,col,k, row1, col1;
		if (dice+pl1>35)
			pl1=pl1;
		else
			pl1=pl1+dice;
		j=(pl1-1)%5;//columns
		j=j+1;
		i=(pl1-1)/5;//rows
		i=i+1;
		if (i%2!=0)
			j=6-j;
		row=i;
		col=j;
//row1, col2 are udpated row and columns
		a = transform(row,col);
		j=(a-1)%5;//columns
		j=j+1;
		i=(a-1)/5;//rows
		i=i+1;
		if (i%2!=0)
			j=6-j;
		row1=i;
		col1=j;
		if (pl1!=a){
			for (j=4;j>0;j--){
				mask(row,col);
				for(k = 20000; k > 0; k--);
				mask(row1, col1);
				for(k = 20000; k > 0; k--);
			}
		}
		else{
			mask(row,col);
		}
		return a;
}
	int getpos2(int dice, int pl1){
			int a;
			volatile unsigned int i,j,row,col,k, row1, col1;
			if (dice+pl1>35)
				pl1=pl1;
			else
				pl1=pl1+dice;
			j=(pl1-1)%5;//columns
			j=j+1;
			i=(pl1-1)/5;//rows
			i=i+1;
			if (i%2!=0)
				j=6-j;
			row=i;
			col=j;

			a = transform(row,col);
			j=(a-1)%5;//columns
			j=j+1;
			i=(a-1)/5;//rows
			i=i+1;
			if (i%2!=0)
				j=6-j;
			row1=i;
			col1=j;
			if (pl1!=a){
				for (j=4;j>0;j--){
					mask2(row,col);
					for(k = 20000; k > 0; k--);
					mask2(row1, col1);
					for(k = 20000; k > 0; k--);
				}
			}
			else{
				mask2(row,col);
			}
			return a;
	}

	int transform(int row, int col)	{
		volatile unsigned int j, r1, r2;
		r1=row;
		r2=col;
		if(row == 2 && col == 3)	{//ladder
			row = 4;
			col = 5;
		}
		else if(row == 4 && col == 1)	{//snake
			row = 1;
			col = 2;
		}
		else if(row == 5 && col == 2)	{//snake
					row = 1;
					col = 3;
				}
		else if(row == 6 && col == 1){//snake
			row = 3;
			col = 2;
		}
		else if(row == 7 && col == 3){//snake
			row = 4;
			col = 5;
		}
		return inverse(row,col);
	}

	void mask(int r, int c){//for player 1
		int x,y;
		volatile unsigned int z;
		P3OUT=0b11111111;//for rows
		P2OUT=0b00000010;//for columns
		r=r-1;
		x=BIT0<<r;//rows -- shift the bits by r
		y=BIT0<<c;//cols -- shift the bits by c
		P3OUT &= ~x;
		if (c==1)	{
			P2OUT =BIT0|BIT1;
		}
		else{
			P2OUT =y|BIT1;
		}
		}
	void mask2(int r, int c){ //for player 2
		volatile unsigned int z;
		P1OUT=BIT1;//KEEP BUTTON 1.1 HIGH
		P4OUT=0b00001111;
		P7OUT=0b00000001;
		P8OUT=0b00000110;
		c=c+1;
		if (r==1){
			P4OUT=0b00001110;
		}
		if (r==2){
				P4OUT=0b00001101;
			}
		if (r==3){
					P4OUT=0b00001011;
				}
		if (r==4){
					P4OUT=0b00000111;
				}
		if (r==5){
			P8OUT=0b00000100;

		}
		if (r==6){
				P8OUT=0b00000010;

			}
		if (r==7){
				P7OUT=0b00000000;

			}

		z=BIT0<<c;
		P1OUT=BIT1|z;

		}


	int inverse(int row, int col)	{
		if ((row%2)!=0)
			col=6-col;
		return (5*(row-1))+col;
	}
