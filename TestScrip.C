{
  gROOT->Reset();

  gInterpreter->LoadMacro("./AllEvents.C");
  TFile *f1 = new TFile("./fluxmodification.root");
  AllEvents mc((TNtuple*)(f1->Get("h101")));

  TLine *line1 = new TLine(0.05, 0.05, 0.80, 0.70);
  line1->SetLineWidth(2);
  line1->SetLineStyle(2);
  line1->SetLineColor(1);

Int_t a = 0;

  Int_t nmc = Int_t((mc.fChain)->GetEntriesFast());

  for (Int_t Event = 0; Event < nmc; Event++)
    {
	mc.GetEntry(Event);
	a++;
	std::cout<< "Event #: " << a <<std::endl;
	for(Int_t z=0; z<mc.npart; z++)
		{
		if(mc.id[z] = 7)
			{
			// Random Number Generator Test
			// to determine distribution of kinetic energy of vector boson decay products
			//TRandom3 r3(0); // unique seed generated using TUUID
			//r3.Rndm(); // Rndm() function generates a pseudo random number distributed between 0 and 1
			//std::cout<< "Random #: "<< r3.Rndm() <<std::endl;
			
			TLorentzVector pion_lf(mc.ini_mom[z][0], mc.ini_mom[z][1], mc.ini_mom[z][2], mc.ini_eng[3]);
			
			//boost pion to  rest frame
			TLorentzVector zero(0,0,0,0);
			TLorentzVector com_p = pion_lf + zero;
			pion_lf.Boost(-com_p.BoostVector());
						
			// Test decay of vector boson in rest frame to photon and pion
			
			// Boost photon and pion to lab frame
			
			// Determine kin
						
			TLorentzVector photon;
			TLorentzVector pion_new;
			pion_new.SetE(134.9766);
			photon.SetE(0);
			vectorB_lf = photon + pion_new;
			
			}
		}
    }
}
