#ifndef SUMWGT_QCDSCALE_PDF_H
#define SUMWGT_QCDSCALE_PDF_H

// <weight id="1001"> muR=0.10000E+01 muF=0.10000E+01 </weight> <-- 0
// <weight id="1002"> muR=0.10000E+01 muF=0.20000E+01 </weight> <-- 1 (muF = 2)
// <weight id="1003"> muR=0.10000E+01 muF=0.50000E+00 </weight> <-- 2 (muF = 0.5)
// <weight id="1004"> muR=0.20000E+01 muF=0.10000E+01 </weight> <-- 3 (muR = 2)
// <weight id="1005"> muR=0.20000E+01 muF=0.20000E+01 </weight>
// <weight id="1006"> muR=0.20000E+01 muF=0.50000E+00 </weight>
// <weight id="1007"> muR=0.50000E+00 muF=0.10000E+01 </weight> <-- 6 (muR = 0.5)
// <weight id="1008"> muR=0.50000E+00 muF=0.20000E+01 </weight>
// <weight id="1009"> muR=0.50000E+00 muF=0.50000E+00 </weight>

vector<double> sumwgt_qcd_pdf(const char* treepath) {

    TFileCollection fc("fc");
    fc.Add(treepath);
    TChain* chain = new TChain("gentree/tree");
    chain->AddFileInfoList(fc.GetList());

    TTreeReader reader(chain);
    TTreeReaderValue<vector<double> > qcdscale(reader, "qcdscale");
    TTreeReaderValue<vector<double> >   pdfunc(reader, "pdfunc"  );
    
    double qcd_1 = 0., qcd_2 = 0., qcd_3 = 0., qcd_6 = 0.;
    double pdf[102]; for (int i = 0; i <= 101; i++) pdf[i] = 0.;
    int events = 0;
    while(reader.Next()) { 

      events++;
      //if (events > 100) break;

      if (events % 50000 == 0) std::cout<<"qcdscale_pdf: events processed "<<events<<"\n";

      qcd_1 += (*qcdscale)[1];
      qcd_2 += (*qcdscale)[2];
      qcd_3 += (*qcdscale)[3];
      qcd_6 += (*qcdscale)[6];

      //std::cout<<"qcdscale "<<(*qcdscale)[1]<<" "<<(*qcdscale)[2]<<" "<<(*qcdscale)[3]<<" "<<(*qcdscale)[6]<<"\n";
      //std::cout<<"qcd "<<qcd_1<<" "<<qcd_2<<" "<<qcd_3<<" "<<qcd_6<<"\n";
      
      for (int i = 0; i <= 101; i++) {
	pdf[i] += (*pdfunc)[i];
	//std::cout<<"pdfunc "<<(*pdfunc)[i]<<"\n";
	//std::cout<<"pdf "<<pdf[i]<<"\n";
      }
    }

    vector<double> weightsum;//0 to 8 (total 9) for qcd; 9 to 110 (total 102) for pdf
    weightsum.push_back(0.);
    weightsum.push_back(qcd_1);
    weightsum.push_back(qcd_2);
    weightsum.push_back(qcd_3);
    weightsum.push_back(0.);
    weightsum.push_back(0.);
    weightsum.push_back(qcd_6);
    weightsum.push_back(0.);
    weightsum.push_back(0.);

    //std::cout<<"weightsum "<<weightsum[1]<<" "<<weightsum[2]<<" "<<weightsum[3]<<" "<<weightsum[6]<<"\n";

    for (int i = 0; i <= 101; i++) weightsum.push_back(pdf[i]);

    return weightsum;
}

#endif