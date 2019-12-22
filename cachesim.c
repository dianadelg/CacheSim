#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

int A;
int B;
int C;
int S;
int b;
int s;
int t;
int memRead=0;
int memWrite=0;
int cacheHit=0;
int cacheMiss=0;
char instruct;
unsigned long address;
int args;
int pf;

typedef struct dSet{
	//set for direct cache
	int valid;
	unsigned long address;
}dSet;

typedef struct fSet{
	//for fully assoc
	int valid;
	int age;
	unsigned long address;

}fSet;

typedef struct line{
	int valid;
	unsigned long address;
	int age;
}line;

typedef struct nSet{
	struct line *lines; //array that hold line type
}nSet;

/*typedef struct aSet{
	line arr[];
}aSet;*/

int StoN(char* number){
	//takes string and converts to number
	int x;
	sscanf(number,"%d",&x);
	//printf("str: %s ..... num: %d\n",number,x);
	return x;
}

int l2(int x){
//takes number and converts log 2 of it
	int k= log(x)/log(2);
	return k;
}

int pow2(int x){
	//checks if power of 2. if not, returns 0
	if((x&(x-1))==0){
		return 1;
	}else{
		return 0;
	}
}

int assoc(char*string){
	if(string[0]=='d'){
		return -1;
		//means direct
	}	

	int c;
	int count=0;
	for(c=0;string[c]!='\0'; c++){
		count++;
	}
	
	if(count==5){
		return 0;
		//0 for fully
	}
	
//takes string with assos:n and gives the n back to user
	int i,j;
		for(i=0;string[i]!='\0';i++){
			while(!((string[i]>='0'&&string[i]<='9')||(string[i]=='\0'))){
			for(j=i;string[j]!='\0';j++){
				string[j]=string[j+1];
			}
		string[j]='\0';
		
	}
		}

	int ret=StoN(string); //turns number into string
	return ret;
}

int main(int argc, char**argv){
	
	if(argc!=7){
		return 0;
	}
	
	//CacheSize
	C=StoN(argv[1]);
//	printf("cache size  %d    ",C);
		if(pow2(C)==0){
			//means not power of 2
			return 0;
		}

	//Associativity
		int assocType=0; //will hold value of assoc type
		if(assoc(argv[2])==-1){
			//direct
		//	A=1; //1 line per set
			assocType=-1;
//			printf("direct   ");	
		}else if(assoc(argv[2])==0){
			//means fully assoc, S=1
		//	S=1;
			assocType=0;
//			printf("full   ");
		}else{
			//means n way 
			//assocType!= -1, 0
			A=assoc(argv[2]);
//			printf("%d way    ",A);
				if(pow2(A)==0){
					return 0; //not pow 2
				}
			assocType=A;
		}

	//Prefetch
		if(argv[3][1]=='1'){
			//means prefetch
			pf=1;
		}else{
			pf=0;
//			printf("no prefetch   ");
		}

	//Replacement --> for now just leave it at FIFO
	
	//Blocksize
	
	B=StoN(argv[5]);
		if(pow2(B)==0){
			return 0;
		} //means not pow of 2
//	printf("block size : %d  ",B);

	//now that everything is properly formatted and correct
	//initialize cache
/*	
	if(A>1){
		return 0;
	}
	//REMOVE THIS LATER
*/	
	FILE*file=fopen(argv[6], "r");
		if(file==NULL){
			return 0;
		}

	if(assocType==-1||assocType==1){
	A=1;
	S=C/(A*B);
	b=l2(B);
	s=l2(S);
	t=48-(s-b);

	dSet*arr=calloc(S,sizeof(dSet));
		if(arr==NULL){
			return 0;
		}

	
	char*line=NULL;
	size_t len=0;
		while((args=getline(&line,&len,file))!=-1){
			if(line[0]=='#'){
				//end of file
				break; //print outputs
			}
			sscanf(line,"%*x%*c %c %lx\n",&instruct,&address); 		
	//now get set index, bits, etc
	unsigned long tagy, temp, setI;
	 //if this doesn't work, also unsigned long
		temp=address>>b;
		setI=temp&((1<<s)-1);
		tagy=temp>>s;		

	//for prefetching
	unsigned long blockC=0;
			
	if(instruct=='R'){
		if(arr[setI].valid==1&&arr[setI].address==tagy){
			cacheHit++;
			
		}else{
			//miss
			memRead++;
			cacheMiss++;
			arr[setI].valid=1;
			arr[setI].address=tagy;//tagy - this is storing b in cache
			if(pf==1){
				//take address
				blockC=address+B;
					unsigned long temp1=blockC>>b;
					unsigned long setI2=temp1&((1<<s)-1);
					unsigned long tagy1=temp1>>s;
						if(arr[setI2].valid==1&&arr[setI2].address==tagy1){
							//no need to prefetch, aleady in cache
						}else{
							//means not in cache i believe --> logic might be wrong
							memRead++;
							arr[setI2].valid=1;
							arr[setI2].address=tagy1;
						}
			}
		}
	}else{
		if(arr[setI].valid==1&&arr[setI].address==tagy){
			if(arr[setI].address==tagy){ //tagy
			
			cacheHit++;
			memWrite++;
			}
		}else{
			cacheMiss++;
			memRead++;
			arr[setI].valid=1;
			arr[setI].address=tagy; //tagy
			memWrite++;
				if(pf==1){
					blockC=address+B;
						unsigned long temp1=blockC>>b;
						unsigned long setI2=temp1&((1<<s)-1);
						unsigned long tagy1=temp1>>s;
							if(arr[setI2].valid==1&&arr[setI2].address==tagy1){
								
							}else{
								//just store c in cache
								memRead++;
								arr[setI2].valid=1;
								arr[setI2].address=tagy1;
							}
				}
		}
	

	}
	//once we have set index: convert to int -> check this
	//then go to arr[set index]
	//check valid bit
	//do the thangs	

		
		//so this is for direct but will change it later
//		printf("%d\n is arr valid\n",arr[0].valid);
			
			//at this point, should have an array of sets
	}
	free(arr);	
}

	if(assocType==0){
		//assoc full:  S=1
		//number of lines per set
		S=1; //where A is the number of lines
		A=C/(S*B);
		b=l2(B);
		s=l2(S);
		t=48-(s-b);
		
	//now make cache
		
	fSet* arr=calloc(A,sizeof(fSet)); //so each spot will be a line 
		//so set is just one, have to go through array and compare tags and valid bits
	
		char* line=NULL;
		size_t len=0;

			while((args=getline(&line,&len,file))!=-1){
				if(line[0]=='#'){
					break; //print outputs)
				}
				
					sscanf(line,"%*x%*c %c %lx\n",&instruct,&address);
					unsigned long tagy, temp;
					temp=address>>b;
					//setI=temp&((1<<s)-1);
					tagy=temp>>s;
						int x; //used as a counter
						int isFull=1;
						int place=0; //an index we can place a thing in	
						int hit=0;
						//int biggestAge=1; //try 0
						
				
				if(instruct=='R'){
					for(x=0;x<A;x++){
						if(arr[x].address!=tagy&&arr[x].valid==0){
							//means a blank space with no data
							isFull=0; //means not full
							place=x; //we can place something at x
						}
						if(arr[x].address==tagy&&arr[x].valid==1){
							cacheHit++;
							hit=1;
						}
					}
					if(hit!=1){
						//means there was no hit
						//so a miss
						cacheMiss++;
						memRead++;
							if(isFull==0){
								//just add it
								arr[place].address=tagy;
								arr[place].valid=1;
								arr[place].age=0;
							}else{
								int oldest=0;
								int hold=0;
								//means full
								for(x=0;x<A;x++){
									if(arr[x].age>=oldest){
										oldest=arr[x].age;
										hold=x;
									}
								}
								arr[hold].age=0;
								arr[hold].address=tagy;
								arr[hold].valid=1;

							}
							for(x=0;x<A;x++){
								if(arr[x].valid==1){
									arr[x].age++;
								}

							}

							if(pf==1){
								unsigned long blockC=address+B;
								unsigned long temp1=blockC>>b;
								unsigned long tagy1=temp1>>s;
								int z=0;
								int isFull2=1;
								int place2=0;
								int hit2=0;
									for(z=0;z<A;z++){
										if(arr[z].address!=tagy1&&arr[z].valid==0){
											isFull2=0;
											place2=z;
										}
										if(arr[z].address==tagy1&&arr[z].valid==1){
											//means a hit!
											hit2=1;
										}

									}
									if(hit2!=1){
										memRead++;
										if(isFull2==0){
											//just add it here
											arr[place2].address=tagy1;
											arr[place2].valid=1;
											arr[place2].age=0;

										}else{
											int oldest2=0;
											int hold2=0;
												for(z=0;z<A;z++){

													if(arr[z].age>=oldest2){
														oldest2=arr[z].age;
														hold2=z;

													}
												}
											arr[hold2].age=0;
											arr[hold2].address=tagy1;
											arr[hold2].valid=1;
										}
																			
	
									}//end of hit
								for(z=0;z<A;z++){
									if(arr[z].valid==1){
										arr[z].age++;
									}
								}// might need this idk 
										

							}

					}
					/*for(x=0;x<A;x++){
						if(arr[x].valid==1){
							arr[x].age++;
						}
					}	*/
				//might need a }

				}else{
					
					for(x=0;x<A;x++){
						if(arr[x].address!=tagy&&arr[x].valid==0){
							isFull=0;
							place=x;
						}
						if(arr[x].address==tagy&&arr[x].valid==1){
							cacheHit++;
							memWrite++;
							hit=1;
						}
					}
					if(hit!=1){
						cacheMiss++;
						memRead++;
						memWrite++;
							if(isFull==0){
								arr[place].address=tagy;
								arr[place].valid=1;
								arr[place].age=0;
							}else{
								int oldest=0;
								int hold=0;
								for(x=0;x<A;x++){
									if(arr[x].age>=oldest){
										oldest=arr[x].age;
										hold=x;
									}
								}
								arr[hold].age=0;
								arr[hold].address=tagy;
								arr[hold].valid=1;
							}
							for(x=0;x<A;x++){
								if(arr[x].valid==1){
									arr[x].age++;
								}
							}
							if(pf==1){
								unsigned long blockC=address+B;
								unsigned long temp1=blockC>>b;
								unsigned long tagy1=temp1>>s;
								int z=0;
								int isFull2=1;
								int place2=0;
								int hit2=0;

									for(z=0;z<A;z++){
										if(arr[z].address!=tagy1&&arr[z].valid==0){
											isFull2=0;
											place2=z;
										}
										if(arr[z].address==tagy1&&arr[z].valid==1){
											hit2=1;
										}
									}
									if(hit2!=1){
										memRead++;
										if(isFull2==0){
											arr[place2].address=tagy1;
											arr[place2].valid=1;
											arr[place2].age=0;
										}else{
											int oldest2=0;
											int hold2=0;
												for(z=0;z<A;z++){
													if(arr[z].age>=oldest2){
														oldest2=arr[z].age;
														hold2=z;
													}
												}
										arr[hold2].age=0;
										arr[hold2].address=tagy1;
										arr[hold2].valid=1;

										}

									}//end of hit
									for(z=0;z<A;z++){
										if(arr[z].valid==1){
											arr[z].age++;
										}
									}
							


							}//break of pf==1	
						
					}
					/*for(x=0;x<A;x++){
						if(arr[x].valid==1){
							arr[x].age++;
						}
					}*/

				}
			}
	}					
	
	if(assocType>1){
		//n way
		//assocType is the number of lines
		A=assocType;
		S=C/(A*B);
		b=l2(B);
		s=l2(S);
		t=48-(s-b);

		nSet*arr=malloc(S*sizeof(nSet));
			if(arr==NULL){
				return 0;
			}
		int x;//counter
		
		for(x=0;x<S;x++){
			arr[x].lines=calloc(A,sizeof(line));

		}
		//so cache should be initialized

		char*line=NULL;
		size_t len=0;

				while((args=getline(&line,&len,file))!=-1){
					if(line[0]=='#'){
						break; //print outputs
					}
				
					sscanf(line,"%*x%*c %c %lx\n",&instruct,&address);
					unsigned long tagy, temp, setI;
					temp=address>>b;
					setI=temp&((1<<s)-1);
					tagy=temp>>s;
		
					int x;
					int isFull=1;
					int place=0;
					int hit=0;

				if(instruct=='R'){
					for(x=0;x<A;x++){
						if(arr[setI].lines[x].address!=tagy&&arr[setI].lines[x].valid==0){
							isFull=0;
							place=x;
						}
						if(arr[setI].lines[x].address==tagy&&arr[setI].lines[x].valid==1){
							cacheHit++;
							hit=1;
						}
					}
					if(hit!=1){
						cacheMiss++;
						memRead++;
							if(isFull==0){
								arr[setI].lines[place].address=tagy;
								arr[setI].lines[place].valid=1;
								arr[setI].lines[place].age=0;
							}else{
								int oldest=0;
								int hold=0;
								for(x=0;x<A;x++){
									if(arr[setI].lines[x].age>=oldest){
										oldest=arr[setI].lines[x].age;
										hold=x;
									}
		
								}
								arr[setI].lines[hold].age=0;
								arr[setI].lines[hold].address=tagy;
								arr[setI].lines[hold].valid=1;
							}
					
					for(x=0;x<A;x++){
						if(arr[setI].lines[x].valid==1){
							arr[setI].lines[x].age++;
						}
					}//end of age
			
					if(pf==1){
						unsigned long blockC=address+B;
						unsigned long temp1=blockC>>b;
						unsigned long setI2=temp1&((1<<s)-1);
						unsigned long tagy1=temp1>>s;
						int z=0;
						int isFull2=1;
						int place2=0;
						int hit2=0;
							for(z=0;z<A;z++){
								if(arr[setI2].lines[z].address!=tagy1&&arr[setI2].lines[z].valid==0){
									isFull2=0;
									place2=z;

								}
								if(arr[setI2].lines[z].address==tagy1&&arr[setI2].lines[z].valid==1){
									hit2=1;
								}

							}
							if(hit2!=1){
								memRead++;
								if(isFull2==0){
									arr[setI2].lines[place2].address=tagy1;
									arr[setI2].lines[place2].valid=1;
									arr[setI2].lines[place2].age=0;
								}else{
									int oldest2=0;
									int hold2=0;
										for(z=0;z<A;z++){
											if(arr[setI2].lines[z].age>=oldest2){
												oldest2=arr[setI2].lines[z].age;
												hold2=z;	
											}	

										}
									arr[setI2].lines[hold2].age=0;
									arr[setI2].lines[hold2].address=tagy1;
									arr[setI2].lines[hold2].valid=1;

								}


							}//end of hit 2 back here and remove aligned one if need be
						for(z=0;z<A;z++){
							if(arr[setI2].lines[z].valid==1){
								arr[setI2].lines[z].age++;
							}
						}


						}//end of pf
					}//end of hit

				}else{
					
					for(x=0;x<A;x++){
						if(arr[setI].lines[x].address!=tagy&&arr[setI].lines[x].valid==0){
							isFull=0;
							place=x;
						}

						if(arr[setI].lines[x].address==tagy&&arr[setI].lines[x].valid==1){
							cacheHit++;
							memWrite++;
							hit=1;
						}

					}

					if(hit!=1){
						cacheMiss++;
						memRead++;
						memWrite++;
							if(isFull==0){
								arr[setI].lines[place].address=tagy;
								arr[setI].lines[place].valid=1;
								arr[setI].lines[place].age=0;
							}
							else{
								int oldest=0;
								int hold=0;
								for(x=0;x<A;x++){
									if(arr[setI].lines[x].age>=oldest){
										oldest=arr[setI].lines[x].age;
										hold=x;
									}
								}
								arr[setI].lines[hold].age=0;
								arr[setI].lines[hold].address=tagy;
								arr[setI].lines[hold].valid=1;
							}
					

						for(x=0;x<A;x++){
							if(arr[setI].lines[x].valid==1){
								arr[setI].lines[x].age++;
							}
						}
					if(pf==1){
						unsigned long blockC=address+B;
						unsigned long temp1=blockC>>b;
						unsigned long setI2=temp1&((1<<s)-1);
						unsigned long tagy1=temp1>>s;
						int z=0;
						int isFull2=1;
						int place2=0;
						int hit2=0;
						
							for(z=0;z<A;z++){
								if(arr[setI2].lines[z].address!=tagy1&&arr[setI2].lines[z].valid==0){
									isFull2=0;
									place2=z;
								}
								if(arr[setI2].lines[z].address==tagy1&&arr[setI2].lines[z].valid==1){
									hit2=1;
								}
							}
							if(hit2!=1){
								memRead++;
								if(isFull2==0){
									arr[setI2].lines[place2].address=tagy1;
									arr[setI2].lines[place2].valid=1;
									arr[setI2].lines[place2].age=0;
								}else{
									int oldest2=0;
									int hold2=0;
										for(z=0;z<A;z++){
											if(arr[setI2].lines[z].age>=oldest2){
												oldest2=arr[setI2].lines[z].age;
												hold2=z;
											}

										}
									arr[setI2].lines[hold2].age=0;
									arr[setI2].lines[hold2].address=tagy1;
									arr[setI2].lines[hold2].valid=1;

								}

							}//end of hit
						for(z=0;z<A;z++){
							if(arr[setI2].lines[z].valid==1){
								arr[setI2].lines[z].age++;
							}
						}

					}//end of pf
					
					
					}//end of hit
				}
	//free(arr);
				}//end of while loop

	}

fclose(file);
//free(arr);

printf("Memory reads: %d\n",memRead);
printf("Memory writes: %d\n",memWrite);
printf("Cache hits: %d\n",cacheHit);
printf("Cache misses: %d\n",cacheMiss);



return 0;

}
