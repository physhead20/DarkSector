{
gROOT->Reset();

gInterpreter->LoadMacro("./AllEvents.C");
TFile *f1 = new TFile("./fluxmodification.root");
AllEvents mc((TNtuple*)(f1->Get("h101")));

TLine *line1 = new TLine(0.05, 0.05, 0.80, 0.70);
line1->SetLineWidth(2);
line1->SetLineStyle(2);
line1->SetLineColor(1);

TH2F *hprob = new TH2F("hprob", "Lifetime Probability",1,-1000,1000,.01,-2,2);
hprob->SetLineColor(kRed);
hprob->SetLineStyle(0);
hprob->SetLineWidth(3);

Int_t a = 0;
Int_t b = 0;
Int_t c = 0;
Int_t d = 0;
Int_t e = 0;

Float_t detstart_distance = 470.0; // distance from target to beginning of MicroBooNE cryostat
Float_t detstop_distance = 480.0; // distance from target to end of MicroBooNE cryostat
Float_t v_light = 299792458.; // speed of light
Float_t alpha_B = 1; // baryonic gauge boson coupling; reasonable range 1 - 10^-8 
Float_t alpha_EM = 7.29735257*pow(10,-3.); // fine structure constant
Float_t m_VB = 0.5; // vector boson mass; units of GeV
Float_t m_pion = 134.9766; // pion^0 rest mass
Float_t pi = 3.141592653589; // pi
Float_t f_pion = 93; // pion decay constants; units of MeV
//===========================================================================

Int_t nmc = Int_t((mc.fChain)->GetEntriesFast());

for(Int_t Evt = 0; Evt < nmc; Evt++)
	{
	mc.GetEntry(Evt);
	a++;
	std::cout<< "Event #" << a <<std::endl;
	for(Int_t z=0; z<mc.npart; z++)
		{
		if(mc.id[z] == 7) // selection for pi^0
			{
			// pion lab frame four momentum
			TLorentzVector pion_lf(mc.ini_mom[z][0], mc.ini_mom[z][1], mc.ini_mom[z][2], mc.ini_eng[3]);
			Float_t imp_lf = pow(pow(pion_lf[3],2.)-pow(pion_lf[0],2.)-pow(pion_lf[1],2.)-pow(pion_lf[2],2.),0.5);
			//std::cout<< "pion invariant lab frame invariant mass: " << imp_lf <<std::endl;
			
			// boost pion to rest frame
			TLorentzVector zero(0,0,0,0);
			TLorentzVector com_p = pion_lf + zero;
			pion_lf.Boost(-com_p.BoostVector());
			
			// pion rest frame four momentum
			TLorentzVector pion_rf(pion_lf[0], pion_lf[1], pion_lf[2], pion_lf[3]);
			Float_t imp_rf = pow(pow(pion_rf[3],2.)-pow(pion_rf[0],2.)-pow(pion_rf[1],2.)-pow(pion_rf[2],2.),0.5);
			//std::cout<< "pion invariant rest frame invariant mass: " << imp_rf <<std::endl;
			
			// convert every 30th pion to a vector boson --> modification needed CHANGE TO ETA:PI^0 RATIO
			b++;
			if(b % 30 == 0)
				{
				c++;
				
				// vector boson rest frame four momentum
				TLorentzVector vectorB_rf(pion_rf[0], pion_rf[1], pion_rf[2], pion_rf[3]);
				vectorB_rf.SetE(0.5); // for 500 MeV rest mass
				Float_t imvb_rf = pow(pow(vectorB_rf[3],2.)-pow(vectorB_rf[0],2.)-pow(vectorB_rf[1],2.)-pow(vectorB_rf[2],2.),0.5);
				//std::cout<< "vector boson four momentum in rest frame: " << vectorB_rf[0] << " " << vectorB_rf[1] << " " << vectorB_rf[2] << " " << vectorB_rf[3] <<std::endl;
				//std::cout << "vector boson rest frame invariant mass: " << imvb_rf <<std::endl;
				
				// boost vector boson to lab frame
				TLorentzVector zero_vb(0,0,0,0);
				TLorentzVector com_vb = vectorB_rf + zero_vb;
				//vectorB_rf.Boost(com_vb.BoostVector() ); boost gives zero momentum three vector
				vectorB_rf.Boost(com_p.BoostVector()); // boost gives non-zero momentum three vector
				
				// vector boson lab frame four momentum
				TLorentzVector vectorB_lf(vectorB_rf[0], vectorB_rf[1], vectorB_rf[2], vectorB_rf[3]);
				Float_t imvb_lf = pow(pow(vectorB_lf[3],2.)-pow(vectorB_lf[0],2.)-pow(vectorB_lf[1],2.)-pow(vectorB_lf[2],2.),0.5);
				//std::cout<< "vector boson four momentum in lab frame: " << vectorB_lf[0] << " " << vectorB_lf[1] << " " << vectorB_lf[2] << " " << vectorB_lf[3] <<std::endl;
				//std::cout << "vector boson lab frame invariant mass: " << imvb_lf <<std::endl;
				
				// detector angular acceptance requirements
				Float_t phi;
				phi = vectorB_lf.Phi();
				if(phi < 0)
					{
					phi = phi*-1;
					}	
				TLorentzVector vectorB_positionlf(mc.ini_pos[z][0], mc.ini_pos[z][1], mc.ini_pos[z][2], 134.9766);
				Float_t xaccang = vectorB_positionlf[0]/470.;
				if(xaccang < 0)
					{
					xaccang = xaccang*-1;
					}
				if(phi <= xaccang)
					{
					d++;
					Float_t yaccang = vectorB_positionlf[1]/470.;
					if(yaccang < 0)
						{
						yaccang = yaccang*-1;
						}
					if (phi <= yaccang)
						{
						e++;
						// decay probability; dL/dt
						Float_t P1;
						Float_t P2; 
						Float_t p_z = vectorB_lf[2];
						Float_t v_z = sqrt((pow(p_z,2.))/(pow(m_VB,2.)+(pow(p_z,2.)/pow(v_light,2.))));
						Float_t t1 = detstart_distance/v_z;
						Float_t t2 = detstop_distance/v_z;
						Float_t gamma = 1/sqrt(1-(pow(v_z,2.)/pow(v_light,2.)));
						Float_t Gamma =(alpha_B*alpha_EM*pow(m_VB,3.))/(96*pow(pi,3.)*pow(f_pion,2.))*pow(1-(pow(m_pion,2.)/pow(m_VB,2.)),3.);
						Float_t Tau = 1/Gamma;
						P1 = exp(-t1/(gamma*Tau));
						P2 = exp(-t2/(gamma*Tau));
						hprob->Fill(v_z, P1);
						//std::cout << "p_z: " << p_z <<std::endl;
						//std::cout << "t1: " << t1 <<std::endl;
						//std::cout << "t2: " << t2 <<std::endl;
						//std::cout << "gamma: " << gamma <<std::endl;
						//std::cout << "Gamma: " << Gamma <<std::endl;
						//std::cout << "Decay probability (for front of detector): " << P1 <<std::endl;
						//std::cout << "Decay probability (for back of detector): " << P2 <<std::endl;
						//std::cout << "Vector v_z: " << v_z <<std::endl;
						
						//boost to vector boson rest frame to decay
						vectorB_lf.Boost(-com_p.BoostVector());
						std::cout<< "Vector boson four momentum: "<< vectorB_lf[0]<< " " << vectorB_lf[1]<< " " << vectorB_lf[2]<< " " << vctorB_lf[3] <<std::endl;
						TLorentzVector photon;
						TLorentzVector pion_new;
						pion_new.SetE(134.9766);
						photon.SetE(0);
						vectorB_lf = photon + pion_new;
						
						// boost photon and vector boson to lab frame
						
						// set kinetic energy distribution based on random number generator -->
						//i.e. random number between 0 and 1 determines kinetic energy distributed to
						//pion 1-random number kinet energy distributed to photon
					
						//determine distribution of kinetic energy of vector boson decay products
						//TRandom3 r3(0); //unique seed generated using TUUID
						//r3.Rndm(); //Rndm() function genertes a pseudo random number distributed between 0 and 1
						//std::cout<< "Random #: " <<r3.Rndm() <<std::endl;
						
						//write out list of photon and pion respective four momentums --> LArSoft
						
						
						
						}
					}
				}
			}
		}
	}

//===========================================================================

std::cout<< "# of pi^0s produced in proton target interaction: " << b <<std::endl;
std::cout<< "# of vector bosons produced in proton target interaction: " << c <<std::endl;
std::cout<< "Vector bosons which meet 1st polar angle cut: " << d <<std::endl;
std::cout<< "Vector bosons which meet 2nd polar angle cut: " << e <<std::endl;

TCanvas* c1 = new TCanvas("c1", "", 200,10,800,600);
c1->SetFillColor(kWhite);
c1->SetTicks();
c1->SetBorderSize(10);

hprob->GetXaxis()->SetTitle("Example (units)");
hprob->GetYaxis()->SetTitle("Example (units)");
hprob->Draw();

//TLatex *t = new TLatex();
//t->SetNDC();
//t->SetTextFont(62);
//t->SetTextSize(0.04);
//t->SetTextAlign(40);
//t->DrawLatex(0.1,0.90, "Preliminary");

//TLegend *leg = new TLegend(0.6,0.65,0.88,0.85);
//leg->SetTextFont(72);
//leg->SetTextSize(0.03);
//leg->SetFillColor(kWhite);
//leg->SetHeader("Example MC");
//leg->AddEntry(histo, "Name", "l");
//leg->Draw();

}
