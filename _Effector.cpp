////////////////////////////////////////////////////////////////////////
int _EffDist( _WAVEHEADINFO *wavinfo, int *data, int *olddata, int level, int gain ){
	
	int i,x;
	int *newData;
	
	newData = (int *)malloc( wavinfo->DataNum*4 );
	
	memcpy( olddata, data, 4*tagWaveinfo.DataNum );
	
	for( i=0; i<wavinfo->DataNum; i++ ) data[i] -=128;
	
	//--------------ˆ—•”•ª‚±‚±‚©‚ç---------------
	for( i=0; i<wavinfo->DataNum; i++ ){
		if( data[i] < 0 ){
			newData[i] = data[i] - gain;
			if( newData[i]<-level+gain/5 )newData[i] = -level-gain/5;
		} else {
			newData[i] = data[i] + gain;
			if( newData[i]>level-gain/5 )newData[i] = level-gain/5;
		}
	}
	//--------------ˆ—•”•ª‚±‚±‚Ü‚Å---------------
	
	
	
	for( i=0; i<wavinfo->DataNum; i++ ){
		newData[i] +=128;
		data[i] += 128;
	}
	
	memcpy( data, newData, 4*tagWaveinfo.DataNum );
	
	_SetWaveFile( "temp.wav", data, dWaveDataR, &tagWaveinfo );

	free( newData );
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
int _EffOverDrive( _WAVEHEADINFO *wavinfo, int *data, int *olddata, int level, int gain ){
	
	int i,j,x,n,z;
	int *newData;
	//int *ecoData;
	
	memcpy( olddata, data, 4*tagWaveinfo.DataNum );
	
	newData = (int *)malloc( wavinfo->DataNum*4 );
	for( i=0;i<wavinfo->DataNum;i++ ){
		newData[i]=0;
		data[i]-=128;
	}
	
	
	//--------------ˆ—•”•ª‚±‚±‚©‚ç---------------
	
	for( i=0; i<wavinfo->DataNum; i++ ){
		if( data[i] < 0 ){
			newData[i] = data[i] * (gain/18);
			if( newData[i]<-level )newData[i] = -level;
		} else {
			newData[i] = data[i] * (gain/18);
			if( newData[i]>level )newData[i] = level;
		}
	}		
	
	
	//---------------ˆ—•”•ª‚±‚±‚Ü‚Å----------------
	
	
	for( i=0; i<wavinfo->DataNum; i++ ){
		if( newData[i] < -128 ) newData[i] = -128;
		if( newData[i] > 128 ) newData[i] = 128;
		newData[i]+=128;
		data[i]+=128;
		//data[i] = newData[i];
	}
	memcpy( data, newData, 4*tagWaveinfo.DataNum );
	
	_SetWaveFile( "temp.wav", data, dWaveDataR, &tagWaveinfo );

	free( newData );
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
int _EffDelay( _WAVEHEADINFO *wavinfo, int *data, int *olddata, int times, int feedback ){
	
	int level;
	int deeps;
	int kaisu;
	int tone;
	int i,j,x;
	int *newData;
	//int *ecoData;
	
	memcpy( olddata, data, 4*tagWaveinfo.DataNum );
	
	deeps = times; kaisu = feedback; level=50; tone=50; 
	
	newData = (int *)malloc( wavinfo->DataNum*4 );
	for( i=0;i<wavinfo->DataNum;i++ ){
		newData[i]=0;
		data[i]-=128;
	}
//--------------------------------------------------------------
	for( j=0; j<wavinfo->DataNum; j++ ){
		newData[j] = data[j];
		x=1;
		for( i=1; i<=kaisu; i++ ){
			if( j-(i*times) < 0 ){
				continue;
			} else {
				newData[j] += data[j-(i*times)];
				x++;
			}
		}
		newData[j] /= x;
	}
	
	/*for( i=0; i<wavinfo->DataNum; i++ ){
		if( i<(deeps) ){
			newData[i] += data[i];
		}else{
			newData[i] += (data[i] + newData[i-deeps]*(1))/2;
			for( j=0; j<kaisu; j++ ){
				if( (i-deeps-((deeps)*j)) > 0 ){
					newData[i] = (data[i] + (int)(newData[i-deeps-((deeps)*j)]*0.8))*0.8;
			}       }
			//newData[i]/=2;
		}
	}*/
		
//--------------------------------------------------------------
	for( i=0; i<wavinfo->DataNum; i++ ){
		if( newData[i] < -128 ) newData[i] = -128;
		if( newData[i] > 128 ) newData[i] = 128;
		newData[i]+=128;
		data[i]+=128;
		//data[i] = newData[i];
	}
	memcpy( data, newData, 4*tagWaveinfo.DataNum );
	
	_SetWaveFile( "temp.wav", data, dWaveDataR, &tagWaveinfo );

	free( newData );
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
int _EffReverb( _WAVEHEADINFO *wavinfo, int *data, int *olddata, int time, int mix ){
	
	int times;
	int timess[5];
	int time_max;
	int i,j,x;
	int *newData;
	//int *ecoData;
	
	memcpy( olddata, data, 4*tagWaveinfo.DataNum );
	
	newData = (int *)malloc( wavinfo->DataNum*4 );
	for( i=0;i<wavinfo->DataNum;i++ ){
		newData[i]=0;
		data[i]-=128;
	}
	
	//--------------ˆ—•”•ª‚±‚±‚©‚ç---------------
	
	times = time*100;
	timess[0] = times*0.8;
	timess[1] = times*0.6;
	timess[2] = times*0.4;
	timess[3] = times*0.2;
	time_max = times+timess[0]+timess[1]+timess[2]+timess[3];
	
	memcpy( newData, data, 4*tagWaveinfo.DataNum );
	for( i=time_max; i<wavinfo->DataNum; i++ ){
		newData[i] = (newData[i] + newData[i-times]);
		newData[i] = (newData[i] + newData[(int)(i-times-timess[0])]*0.6);
		newData[i] = (newData[i] + newData[(int)(i-times-timess[1])]*0.7);
		newData[i] = (newData[i] + newData[(int)(i-times-timess[2])]*0.4);
		newData[i] = (newData[i] + newData[(int)(i-times-timess[3])]*0.5)/4;
		//newData[i] = (int)((float)newData[i] * 1.25);
			
	}
	for( i=0; i<wavinfo->DataNum; i++ ){
		newData[i] *= 3;
	}
	
	//--------------ˆ—•”•ª‚±‚±‚Ü‚Å---------------
	
	for( i=0; i<wavinfo->DataNum; i++ ){
		if( newData[i] < -128 ) newData[i] = -128;
		if( newData[i] > 128 ) newData[i] = 128;
		newData[i]+=128;
		data[i]+=128;
		//data[i] = newData[i];
	}
	memcpy( data, newData, 4*tagWaveinfo.DataNum );
	
	_SetWaveFile( "temp.wav", data, dWaveDataR, &tagWaveinfo );

	free( newData );
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////
int _EffColas( _WAVEHEADINFO *wavinfo, int *data, int *olddata, int times ){
	
	int level;
	int deeps;
	int kaisu;
	int tone;
	int i,j,x;
	int *newData;
	//int *ecoData;
	
	memcpy( olddata, data, 4*tagWaveinfo.DataNum );
	
	
	deeps = 1300+times;	//1323‚ªŠî€
	level=50; tone=50; 
	
	newData = (int *)malloc( wavinfo->DataNum*4 );
	for( i=0;i<wavinfo->DataNum;i++ ){
		newData[i]=0;
		data[i]-=128;
	}
	
	
	//--------------ˆ—•”•ª‚±‚±‚©‚ç---------------
	
	
	memcpy( newData, data, 4*tagWaveinfo.DataNum );
	for( i=deeps; i<wavinfo->DataNum; i++ ){
		newData[i] = (data[i] + data[i-deeps])/2;
		
	}for( i=deeps; i<wavinfo->DataNum; i++ ){
		newData[i] = (data[i] + data[i-deeps])/2;
		
	}
	
	
	//---------------ˆ—•”•ª‚±‚±‚Ü‚Å----------------
	
	
	for( i=0; i<wavinfo->DataNum; i++ ){
		if( newData[i] < -128 ) newData[i] = -128;
		if( newData[i] > 128 ) newData[i] = 128;
		newData[i]+=128;
		data[i]+=128;
		//data[i] = newData[i];
	}
	memcpy( data, newData, 4*tagWaveinfo.DataNum );
	
	_SetWaveFile( "temp.wav", data, dWaveDataR, &tagWaveinfo );

	free( newData );
	
	return 0;
}

///////////////////////////////////////////////////////////////////////
int _EffVibrate( _WAVEHEADINFO *wavinfo, int *data, int *olddata, int time ){
	
	int vib=time;
	
	int i,j,x,n;
	float z;
	int now;
	int *newData;
	//int *ecoData;
	
	memcpy( olddata, data, 4*tagWaveinfo.DataNum );
	
	newData = (int *)malloc( wavinfo->DataNum*4 );
	for( i=0;i<wavinfo->DataNum;i++ ){
		newData[i]=0;
		data[i]-=128;
	}
	
	
	//--------------ˆ—•”•ª‚±‚±‚©‚ç---------------
	n=time; now=n/2;
	z=0; x=0;
	for( i=0; i<wavinfo->DataNum-10; i++ ){
		switch(x){
			case 1:
				z+=1.10;
				break;
			case 2:
				z+=0.90;
				break;
		}
		newData[i] += data[(int)z];
		if( now++ == n ){
			if( x==1 ){
				x=2;
			} else {
				x=1;
			}
			now=0;
		}
		
	}
	//---------------ˆ—•”•ª‚±‚±‚Ü‚Å----------------
	
	
	for( i=0; i<wavinfo->DataNum; i++ ){
		if( newData[i] < -128 ) newData[i] = -128;
		if( newData[i] > 128 ) newData[i] = 128;
		newData[i]+=128;
		data[i]+=128;
		//data[i] = newData[i];
	}
	memcpy( data, newData, 4*tagWaveinfo.DataNum );
	
	_SetWaveFile( "temp.wav", data, dWaveDataR, &tagWaveinfo );

	free( newData );
	
	return 0;
}

////////////////////////////////////////////////////////////////////
int _EffComp( _WAVEHEADINFO *wavinfo, int *data, int *olddata, int level ){
	
	int i,j,x,n,z;
	int *newData;
	//int *ecoData;
	
	memcpy( olddata, data, 4*tagWaveinfo.DataNum );
	
	newData = (int *)malloc( wavinfo->DataNum*4 );
	for( i=0;i<wavinfo->DataNum;i++ ){
		newData[i]=0;
		data[i]-=128;
	}
	
	
	//--------------ˆ—•”•ª‚±‚±‚©‚ç---------------
	
	long avep=0, avem=0;
	int p=0, m=0;		//+-ƒf[ƒ^‚Ì”
	int limit=level;
	int limit2=30;
	int limit3=-level;
	int limit4=limit2*(-1);
	float limitter=(limit+limit2)/2;
	float limitter2=(limit3+limit4)/2;
	int	Frag=0;
	
	for( i=0; i<wavinfo->DataNum; i++ ){
      	if( data[i] > limit ){
			newData[i] = ((data[i]-limit)*0.6)+limit;
		} else if( data[i] < limit3 ){
			newData[i] = ((data[i]-limit3)*0.6)+limit3;
		} else {
			newData[i] = data[i];
		}
	}
	
	/*for( i=0; i<wavinfo->DataNum; i++ ){
		
		if( data[i] > 0 ){
			if( Frag == 1 && data[i] > limitter ){
				Frag = 0;
			} else if( Frag == 2 && data[i] < limitter ){
				Frag = 0;
			}
			if( data[i] > limit ){
				Frag = 2;
			} else if( data[i] < limit2 ){
				Frag = 1;
			}
		} else if ( data[i] < 0 ){
			if( Frag == 1 && data[i] > limitter2 ){
				Frag = 0;
			} else if( Frag == 2 && data[i] < limitter2 ){
				Frag = 0;
			}
			if( data[i] < limit3 ){
				Frag = 2;
			} else if( data[i] > limit4 ){
				Frag = 1;
			}
		}
		
		if( Frag == 0 ){
			newData[i] = data[i];
		} else if( Frag == 1 ){
			newData[i] = data[i] * 1.2;
		} else if( Frag == 2 ){
			newData[i] = data[i] * 0.9;
		}
	}*/
		
		
	
	
	//---------------ˆ—•”•ª‚±‚±‚Ü‚Å----------------
	
	
	for( i=0; i<wavinfo->DataNum; i++ ){
		if( newData[i] < -128 ) newData[i] = -128;
		if( newData[i] > 128 ) newData[i] = 128;
		newData[i]+=128;
		data[i]+=128;
		//data[i] = newData[i];
	}
	memcpy( data, newData, 4*tagWaveinfo.DataNum );
	
	_SetWaveFile( "temp.wav", data, dWaveDataR, &tagWaveinfo );

	free( newData );
	
	return 0;
}

////////////////////////////////////////////////////////////////////////
int _EffRobo( _WAVEHEADINFO *wavinfo, int *data, int *olddata ){
	
	int robo;
	int i,j,x;
	int *newData;
	//int *ecoData;
	
	memcpy( olddata, data, 4*tagWaveinfo.DataNum );
	
	robo = 3; 
	
	newData = (int *)malloc( wavinfo->DataNum*4 );
	for( i=0;i<wavinfo->DataNum;i++ ){
		newData[i]=0;
		data[i]-=128;
	}
	
	//--------------ˆ—•”•ª‚±‚±‚©‚ç---------------
	
	memcpy( newData, data, 4*tagWaveinfo.DataNum );
	for( j=0; j<=robo; j++ ){
	for( i=100; i<wavinfo->DataNum; i++ ){
		newData[i] = (newData[i] + newData[i-100])/2;
		
	}}
	for( i=0; i<wavinfo->DataNum; i++ ){
		newData[i] *= 3;
	}
	
	//--------------ˆ—•”•ª‚±‚±‚Ü‚Å---------------
	
	for( i=0; i<wavinfo->DataNum; i++ ){
		newData[i]+=128;
		data[i]+=128;
		//data[i] = newData[i];
	}
	memcpy( data, newData, 4*tagWaveinfo.DataNum );
	
	_SetWaveFile( "temp.wav", data, dWaveDataR, &tagWaveinfo );

	free( newData );
	
	return 0;
}