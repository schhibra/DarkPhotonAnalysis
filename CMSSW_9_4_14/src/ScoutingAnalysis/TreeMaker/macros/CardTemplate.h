#ifndef CARDTEMPLETE_H
#define CARDTEMPLETE_H

#include <sstream>

std::string createCardTemplate(double mh, std::string etacat, std::string workspacefilename) {

    std::stringstream binss;
    binss << "   " << getChannelName(etacat) << "      ";
    std::string bin = binss.str();

    std::string binname = getChannelName(etacat);
    std::string spdfstart = "";
    std::string bpdfstart = "";

    spdfstart = "w:sig_mass_";
    bpdfstart = "w:bkg_mass_";

    std::string data_obs_str = "w:data_obs";

    workspacefilename.erase(0,19);

    std::stringstream card;
    card << "imax *                                                                                                                        \n";
    card << "jmax *                                                                                                                        \n";
    card << "kmax *                                                                                                                        \n";

    card << "------------                                                                                                                  \n";
    card << "shapes ggH      " << bin << "     " << workspacefilename << "     " << spdfstart << "dp_"  << binname << "_pdf                \n";
    card << "shapes bkg      " << bin << "     " << workspacefilename << "     " << bpdfstart << ""     << binname << "_pdf                \n";
    card << "shapes data_obs " << bin << "     " << workspacefilename << "     " << data_obs_str                                      <<  "\n";
    card << "------------                                                                                                                  \n";
    card << "bin             " << bin                                                                                                 <<  "\n";
    card << "observation        -1                                                                                                         \n";
    card << "------------                                                                                                                  \n";

    card << "bin                               " << bin   <<     bin    <<  "\n";
    card << "process                              dp_1           bkg         \n";
    card << "process                              -1             1           \n";
    card << "rate                                  1             1           \n";
    card << "------------\n";
    card << "## Dark photon mass : " << mh << std::endl;
    
    card << "lumi_13TeV                lnN        1.026          -           \n";
    card << "QCDscale                  lnN        1.10           -           \n";
    card << "pdf_gg                    lnN        1.10           -           \n";
    card << "CMS_eff_m_sel             lnN        1.015          -           \n";

    return card.str();
}

#endif
