int _GetWaveData( int *wavedataL, int *wavedataR, FILE *file ){
	
	int i;
	int chan;
	int sample;
	
	chan = tagWaveinfo.Channels;
	sample = tagWaveinfo.SampleBit / 8;
	
	//printf( "check1: %d\n", chan );
	//printf( "check2: %d\n", sample );
	
	/* ”z—ñ‚É–³M†’l‚ğ‘ã“ü */
	if( sample == 1 ){
		for( i=0; i<tagWaveinfo.DataNum; i++ ){
			//printf( "check: %d\n", i ); 
			wavedataL[i] = 128;
			wavedataR[i] = 128;
		}
	} else if( sample == 2 ){
		for( i=0; i<tagWaveinfo.DataNum; i++ ){
			wavedataL[i] = 0;
			wavedataR[i] = 0;
		}
	}
	
	i=0;
	//printf( "checkcheck: %d\n", wavinfo->Offset );
	while(1){
		fseek(file, tagWaveinfo.Offset+(i*sample*chan), SEEK_SET);
		
		if( sample == 1 ){
			wavedataL[i] = _getUbyte(file);
		} else if( sample == 2 ){
			wavedataL[i] = _getUshort(file);
		}
		
		fseek(file, sample, SEEK_CUR);
		if( chan == 2 ){
			if( sample == 1 ){
				wavedataR[i] = _getUbyte(file);
			} else if( sample == 2 ){
				wavedataR[i] = _getUshort(file);
			}
		}
		//printf( "%d  L=%d, R=%d\n", i,wavedataL[i], wavedataR[i] );
		//printf( "%d: %d\n", i, wavedataL[i] );
		
		//printf( "check %d  ", i );
		//printf( "check %d  \n", wavedataL[i] );
		
		if(i == tagWaveinfo.DataNum ) break;
		i++;
	}
	
}