#include <boost/program_options.hpp>
#include <sstream>

#include "Settings.h"

namespace po = boost::program_options;

Settings::BadArgs::BadArgs(const std::string& msg) noexcept
: msg(msg)
{}

const char* Settings::BadArgs::what() const noexcept
{
	return msg.c_str();
}


Settings::Settings(int argc, char** argv)
: inputFile()
, outputFile()
, htBmSettings()
, wieBmSettings()
{
	po::options_description required_desc("Required options");
	required_desc.add_options()
		("input,i", po::value(&inputFile)->required(), "Input file")
		("output,o", po::value(&outputFile)->required(), "Output file")
		("sigma,s", po::value(&sigma)->required(), "Standard deviation of additive white Gaussian noise");

	po::options_description ht_desc("Block matching options (hard threshold step)");
	ht_desc.add_options()
		("ht-block-size", po::value(&(htBmSettings.blockSize))->default_value(8), "Block size (HT)")
		("ht-temporal-extent", po::value(&(htBmSettings.extent))->default_value(4), "Max temporal extent (HT)")
		("ht-n-step", po::value(&(htBmSettings.NStep))->default_value(6), "Step between reference blocks (HT)")
		("ht-gamma-p", po::value(&(htBmSettings.gammaP))->default_value(0.3), "Velocity coefficient (HT)")
		("ht-n-s", po::value(&(htBmSettings.Ns))->default_value(11), "Block search window size (HT)")
		("ht-gamma-w", po::value(&(htBmSettings.gammaW))->default_value(0.5), "Block search window scaling factor (HT)")
		("ht-sigma-w", po::value(&(htBmSettings.sigmaW))->default_value(1.0), "Block search window tuning parameter (HT)")
		("ht-n-g", po::value(&(htBmSettings.Ng))->default_value(19), "Volume search window size (HT)")
		("ht-m", po::value(&(htBmSettings.M))->default_value(32), "Number of volumes in group (HT)");

	po::options_description wie_desc("Block matching options (Wiener step)");
	wie_desc.add_options()
		("wie-block-size", po::value(&(wieBmSettings.blockSize))->default_value(7), "Block size (Wie)")
		("wie-temporal-extent", po::value(&(wieBmSettings.extent))->default_value(4), "Max temporal extent (Wie)")
		("wie-n-step", po::value(&(wieBmSettings.NStep))->default_value(4), "Step between reference blocks (Wie)")
		("wie-gamma-p", po::value(&(wieBmSettings.gammaP))->default_value(0.3), "Velocity coefficient (Wie)")
		("wie-gamma-d", po::value(&(wieBmSettings.gammaD))->default_value(0.005), "Block distance penaltization coefficient (Wie)")
		("wie-n-s", po::value(&(wieBmSettings.Ns))->default_value(11), "Block search window size (Wie)")
		("wie-gamma-w", po::value(&(wieBmSettings.gammaW))->default_value(0.5), "Block search window scaling factor (Wie)")
		("wie-sigma-w", po::value(&(wieBmSettings.sigmaW))->default_value(1.0), "Block search window tuning parameter (Wie)")
		("wie-tau-traj", po::value(&(wieBmSettings.tauTraj))->default_value(1), "Block similarity threshold (Wie)")
		("wie-tau-match", po::value(&(wieBmSettings.tauMatch))->default_value(13.5), "Volume similarity threshold (Wie)")
		("wie-n-g", po::value(&(wieBmSettings.Ng))->default_value(27), "Volume search window size (Wie)")
		("wie-m", po::value(&(wieBmSettings.M))->default_value(8), "Number of volumes in group (Wie)");

	po::options_description help_desc("Other");
	help_desc.add_options()
		("help,h", "Print help message");

	po::options_description cmdline_options;
	cmdline_options.add(required_desc).add(ht_desc).add(wie_desc);

	po::variables_map vm;

	try {
		po::store(po::command_line_parser(argc, argv).options(help_desc).allow_unregistered().run(), vm);
		po::notify(vm);

		if (argc < 2 || vm.count("help")) {
			std::stringstream ss;
			ss << required_desc << std::endl;
			ss << ht_desc << std::endl;
			ss << wie_desc << std::endl;
			ss << help_desc << std::endl;
			throw BadArgs(ss.str());
		}

		po::store(po::command_line_parser(argc, argv).options(cmdline_options).run(), vm);
		po::notify(vm);

		htBmSettings.gammaD = 0.0005 * sigma * sigma - 0.0059 * sigma + 0.04;
		htBmSettings.tauTraj = 0.0047 * sigma * sigma + 0.0676 * sigma + 0.4564;
		htBmSettings.tauMatch = 0.0171 * sigma * sigma + 0.4520 * sigma + 47.9294;
	} catch (const po::error& e) {
		throw BadArgs(e.what());
	}
}
