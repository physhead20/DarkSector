{
gROOT->Reset();


// ==========================================
// === Loading your .C File and ROOT file === 
// ==========================================
gInterpreter->LoadMacro("./AllEvents.C"); 
TFile *f1=new TFile("./fluxmodification.root");   
AllEvents mc((TNtuple*)(f1->Get("h101")));

// ##################################################
// # Drawing Lines for horizontal and vertical axis #
// ##################################################
TLine *line1 = new TLine(0.05,0.05,0.80,0.70);
line1->SetLineWidth(2);
line1->SetLineStyle(2);
line1->SetLineColor(1);

//TH1F *hid = new TH1F("hid", "Particle Distribution",40,0,20);
//hid->SetLineColor(kBlue);
//hid->SetLineStyle(0);
//hid->SetLineWidth(2);

TH1F *hmompion = new TH1F("hmompion", "Pion Momentum Distribution", 1000, 0, 100);
hmompion->SetLineColor(kBlack);
hmompion->SetLineStyle(0);
hmompion->SetLineWidth(2);

TH1F *hmomboson = new TH1F("hmomboson", "Vector Boson Momentum Distribution",1000,0, 100);
hmomboson->SetLineColor(kBlue);
hmomboson->SetLineStyle(0);
hmomboson->SetLineWidth(2);

TH1F *hmomdetbos = new TH1F("hmomdetbos", "Vector Boson Momentum in Detector",1000, 0, 100);
hmomdetbos->SetLineColor(kRed);
hmomdetbos->SetLineStyle(0);
hmomdetbos->SetLineWidth(2);

//TH1F *hangacc = new TH1F("hangacc", "Vector Boson Distribution Angles", 550, -0.1, 1);
//hangacc->SetLineColor(kRed);
//hangacc->SetLineStyle(0);
//hangacc->SetLineWidth(1);

//TH1F *hangall = new TH1F("hangall", "", 110, -0.1, 1);
//hangall->SetLineColor(kBlue);
//hangall->SetLineStyle(0);
//hangall->SetLineWidth(1);

Int_t nmc = Int_t((mc.fChain)->GetEntriesFast());
Int_t h=0;
Int_t t=0;
Int_t s=0;
Int_t r=0;
Int_t c=0;


//for(Int_t MCevt = 0; MCevt < nmc; MCevt++)
   //{ 
   //mc.GetEntry(MCevt);
   //for(Int_t k=0; k<mc.npart; k++)
   	//{
	//hid->Fill(mc.id[k]);
	//}
   //}


for(Int_t MontCevt = 0; MontCevt < nmc; MontCevt++)
   {
   mc.GetEntry(MontCevt);
   h++; 
   std::cout<<"Event # =" << h <<std::endl;  
   for(Int_t m=0; m<mc.npart; m++)
   	{		
	//if(mc.id[m] == 8 || mc.id[m] == 9) //selection for pi^+ or pi^-
	if(mc.id[m] == 7) //selection for pi^0	
		{	
		// pion lab frame four momentum
		TLorentzVector pion_mlf(mc.ini_mom[m][0], mc.ini_mom[m][1], mc.ini_mom[m][2], mc.ini_eng[m]);
		//std::cout<< "pion four-momentum in lab frame:" << pion_mlf[0]<<" "<< pion_mlf[1]<<" "<<pion_mlf[2]<<" "<< pion_mlf[3] <<std::endl;
		Float_t im_lf = pow(pion_mlf[3],2.0)-pow(pion_mlf[0],2.0)-pow(pion_mlf[1],2.0)-pow(pion_mlf[2],2.0);
		//std::cout<< "pion invariant mass lab frame: " << im_lf << std::endl; //not actually the invariant mass (the square root of this term is the invariant mass)
		
		// momentum magnitude (pion lab frame)
		TVector3 p(mc.ini_mom[m][0], mc.ini_mom[m][1], mc.ini_mom[m][2]);
		Float_t magp = 100*(p.Mag());
		hmompion->Fill(magp);	
			
		TLorentzVector zero_m(0,0,0,0);		
		TLorentzVector com = pion_mlf + zero_m;
		
		// boost pion to rest frame
		pion_mlf.Boost(-com.BoostVector());
		
		// pion rest frame four momentum
		TLorentzVector pion_rf(pion_mlf[0], pion_mlf[1], pion_mlf[2], pion_mlf[3]);
		//std::cout<< "pion four-momentum in rest frame:" << pion_rf[0]<<" "<<pion_rf[1]<<" "<<pion_rf[2]<<" "<<pion_rf[3] <<std::endl;
		Float_t im_rf = pow(pion_rf[3],2.0)-pow(pion_rf[0],2.0)-pow(pion_rf[1],2.0)-pow(pion_rf[2],2.0);
		//std::cout<< "pion invariant mass rest frame: " << im_rf << std::endl; //not actually the invariant mass (the square root of this term is the invariant mass)
				
		// convert every 30th pion to a vector boson --> modification needed CHANGE TO ETA:PI^0 RATIO
		t++; //the t-th pion which meets the criteria	
		if(t % 30 == 0)	
			{
			s++;
			// vector boson rest frame four momentum
			TLorentzVector vectorB_rf(pion_rf[0], pion_rf[1], pion_rf[1], pion_rf[2]);
			vectorB_rf.SetE(0.5); //for 500 MeV
			//std::cout<< "vector boson four-momentum in the pion rest frame: " << vectorB_rf[0]<<" "<<vectorB_rf[1]<<" "<<vectorB_rf[2]<<" "<<vectorB_rf[3] <<std::endl;
			Float_t im_rfv = pow(vectorB_rf[3],2.0)-pow(vectorB_rf[0],2.0)-pow(vectorB_rf[1],2.0)-pow(vectorB_rf[2],2.0);
			//std::cout<< "vector boson invariant mass rest frame: " << im_rfv << std::endl; //not actually the invariant mass (the square root of this term is the invariant mass)
			
			// boost vector boson to lab frame
			TLorentzVector zero_mvb(0,0,0,0);
			TLorentzVector com_vb = vectorB_rf + zero_mvb;
			vectorB_rf.Boost(com_vb.BoostVector() );
			
			// vector boson lab frame four momentum
			TLorentzVector vectorB_lf(vectorB_rf[0], vectorB_rf[1], vectorB_rf[2], vectorB_rf[3]);
			//std::cout<< "vector boson four-momentum in lab frame: " << vectorB_lf[0]<<" "<<vectorB_lf[1]<<" "<<vectorB_lf[2]<<" "<<vectorB_lf[3] <<std::endl;
			Float_t im_lfv = pow(vectorB_lf[3],2.0)-pow(vectorB_lf[0],2.0)-pow(vectorB_lf[1],2.0)-pow(vectorB_lf[2],2.0);
			//std::cout<< "vector boson invariant mass lab frame: " << im_lfv << std::endl; //not actually the invariant mass (the square root of this term is the invariant mass)
			
			// momentum magnitude (vector boson lab frame) !! may need to change to vectorB_lf... notation!!!
			TVector3 y(vectorB_rf[0], vectorB_rf[1], vectorB_rf[2]);
			Float_t magvb = 100*(y.Mag());
			hmomboson->Fill(magvb); 
			
			// vector boson position
			TLorentzVector vectorB_plf(mc.ini_pos[m][0], mc.ini_pos[m][1], mc.ini_pos[m][2], 134.9766);
			
			// detector angular acceptance requirements (phi < detector angular acceptance); where phi derived from four-momentum polar angle; detector angularacceptance governed by x/detector distance and y/detector distance
			Float_t phi;
			phi = vectorB_lf.Phi();
			if (phi < 0)
				{
				phi = phi*-1;
				}
			
			Float_t xaccang = vectorB_plf[0]/470; // sin(theta) = theta (using small angle approximation)
			if (xaccang < 0)
				{
				xaccang = xaccang*-1;
				}
			
			Float_t phi_all;
			phi_all = vectorB_lf.Phi();
			if (phi_all < 0)
				{
				phi_all = phi_all*-1;
				}
			// hangall->Fill(phi_all); // polar angle distribution for all vector bosons 
			
			if (phi <= xaccang)
				{
				r++;
				Float_t yaccang = vectorB_plf[1]/470;
				if (yaccang < 0)
					{
					yaccang = yaccang*-1;
					}
				if (phi <= yaccang)
					{
					Float_t magvbsel = 100*(y.Mag());
					hmomdetbos->Fill(magvbsel); // momentum magnitude for vector bosons which reach the detector
					// hangacc->Fill(phi); // polar angle distribution for all vector bosons that reach the detector 
					c++;
					}	
				}
			
			} 
		}
     	}
   }  
  
//std::cout<<"Number of pions = "<< n_pion <<std::endl;
std::cout<<"# of pions = " << t <<std::endl;
std::cout<<"# of vector bosons =" << s <<std::endl;
std::cout<<"passed first if angle statement = " << r <<std::endl;
std::cout<<"passed second if angle statement = " << c <<std::endl;
   
//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////   Drawing your Histograms    ///////////////////////////////
TCanvas* c1 = new TCanvas("c1", " ", 200,10,800,600);
c1->SetFillColor(kWhite);
c1->SetTicks();
c1->SetBorderSize(10);   

//hid->Draw();
//hid->GetXaxis()->SetBinLabel(2,"#gamma");
//hid->GetXaxis()->SetBinLabel(4,"e^{+}");
//hid->GetXaxis()->SetBinLabel(6, "e^{-}");
//hid->GetXaxis()->SetBinLabel(8,"#nu_{e,#mu}");
//hid->GetXaxis()->SetBinLabel(10,"#mu^{+}");
//hid->GetXaxis()->SetBinLabel(12,"#mu^{-}");
//hid->GetXaxis()->SetBinLabel(14,"#pi^{0}");
//hid->GetXaxis()->SetBinLabel(16,"#pi^{+}");
//hid->GetXaxis()->SetBinLabel(18,"#pi^{-}");
//hid->GetXaxis()->SetBinLabel(20,"K^{0}_{L}");
//hid->GetXaxis()->SetBinLabel(22,"K^{+}");
//hid->GetXaxis()->SetBinLabel(24,"K^{-}");
//hid->GetXaxis()->SetBinLabel(26,"n");
//hid->GetXaxis()->SetBinLabel(28,"p");
//hid->GetXaxis()->SetBinLabel(30,"#bar{p}");
//hid->GetXaxis()->SetBinLabel(32,"K^{0}_{S}");
//hid->GetXaxis()->SetBinLabel(34,"");
//hid->GetXaxis()->SetBinLabel(36,"#lambda);
//hid->GetXaxis()->SetBinLabel(38,"#sigma^+");
//hid->GetXaxis()->SetBinLabel(40,"sigma^0");
//hid->GetXaxis()->SetTitle("Particle Type");
//hid->GetYaxis()->SetTitle("# of Events");
//gStyle->SetLabelSize(10);
//hid->UseCurrentStyle();
//hid->GetXaxis()->SetTitle("Particle Type");
//hid->GetYaxis()->SetTitle("# of Events");
//hid->Draw();

hmompion->Draw();
hmompion->GetXaxis()->SetTitle("Momentum (MeV/c)");
hmompion->GetYaxis()->SetTitle("# of Events");
hmomboson->Draw("same");
hmomdetbos->Draw("same");


//hangall->Draw();
//hangall->GetXaxis()->SetTitle("|#phi|");
//hangall->GetYaxis()->SetTitle("# of Events");
//hangacc->Draw("same");


// ##########################
// # Setting a LaTeX Header #
// ##########################
//TLatex *t = new TLatex();
//t->SetNDC();
//t->SetTextFont(62);
//t->SetTextSize(0.04);
//t->SetTextAlign(40);
//t->DrawLatex(0.1,0.90,"MicroBooNE Preliminary");

// ######################
// # Setting the Legend #
// ######################

TLegend *leg = new TLegend(0.6,0.65,0.88,0.85);
leg->SetTextFont(72);
leg->SetTextSize(0.03);
//leg->SetTextAlign(12);
leg->SetFillColor(kWhite);
//leg->SetLineColor(kWhite);
//leg->SetShadowColor(kWhite);
//leg->SetHeader("#nu-mode MC");
leg->AddEntry(hmompion, "#pi^{0}", "l");
leg->AddEntry(hmomboson, "All V_{B}", "l");
leg->AddEntry(hmomdetbos, "V_{B} that reach detector", "l");
leg->Draw();

//TLegend *leg = new TLegend(0.6,0.65,1.12,0.85);
//leg->SetTextFont(72);
//leg->SetTextSize(0.03);
//leg->SetFillColor(kWhite);
//leg->AddEntry(hangall, "All V_{B}");
//leg->AddEntry(hangacc, "V_{B} that reach detector");
//leg->Draw();

 
}//<---End File
