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
{
	po::options_description required_desc("Required options");
	required_desc.add_options()
		("sigma,s", po::value(&sigma)->required(), "Standard deviation of additive white Gaussian noise")
		("input,i", po::value(&inputFile)->required(), "Input file")
		("output,o", po::value(&outputFile)->required(), "Output file");

	po::options_description additional_desc("Additional options (HT - hard threshold step, Wie - Wiener step)");
	additional_desc.add_options()
		("ht-block-size", po::value(&htBlockSize)->default_value(htBlockSize), "Block size (HT)")
		("wie-block-size", po::value(&wieBlockSize)->default_value(wieBlockSize), "Block size (Wie)")
		("ht-temporal-extent", po::value(&htExtent)->default_value(htExtent), "Max temporal extent (HT)")
		("wie-temporal-extent", po::value(&wieExtent)->default_value(wieExtent), "Max temporal extent (Wie)")
		("ht-n-step", po::value(&htNStep)->default_value(htNStep), "Step between reference blocks (HT)")
		("wie-n-step", po::value(&wieNStep)->default_value(wieNStep), "Step between reference blocks (Wie)")
		("ht-gamma-p", po::value(&htGammaP)->default_value(htGammaP), "Velocity coefficient (HT)")
		("wie-gamma-p", po::value(&wieGammaP)->default_value(wieGammaP), "Velocity coefficient (Wie)")
		("wie-gamma-d", po::value(&wieGammaD)->default_value(wieGammaD), "Block distance penaltization coefficient (Wie)")
		("ht-n-s", po::value(&htNs)->default_value(htNs), "Block search window size (HT)")
		("wie-n-s", po::value(&wieNs)->default_value(wieNs), "Block search window size (Wie)")
		("ht-gamma-w", po::value(&htGammaW)->default_value(htGammaW), "Block search window scaling factor (HT)")
		("wie-gamma-w", po::value(&wieGammaW)->default_value(wieGammaW), "Block search window scaling factor (Wie)")
		("ht-sigma-w", po::value(&htSigmaW)->default_value(htSigmaW), "Block search window tuning parameter (HT)")
		("wie-sigma-w", po::value(&wieSigmaW)->default_value(wieSigmaW), "Block search window tuning parameter (Wie)")
		("wie-tau-traj", po::value(&wieTauTraj)->default_value(wieTauTraj), "Block similarity threshold (Wie)")
		("wie-tau-match", po::value(&wieTauMatch)->default_value(wieTauMatch), "Volume similarity threshold (Wie)")
		("ht-n-g", po::value(&htNg)->default_value(htNg), "Volume search window size (HT)")
		("wie-n-g", po::value(&wieNg)->default_value(wieNg), "Volume search window size (Wie)")
		("ht-m", po::value(&htM)->default_value(htM), "Number of volumes in group (HT)")
		("wie-m", po::value(&wieM)->default_value(wieM), "Number of volumes in group (Wie)");


	po::options_description help_desc("Other");
	help_desc.add_options()
		("help,h", "Print help message");

	po::options_description cmdline_options;
	cmdline_options.add(required_desc).add(additional_desc);

	po::variables_map vm;

	try {
		po::store(po::command_line_parser(argc, argv).options(help_desc).allow_unregistered().run(), vm);
		po::notify(vm);

		if (argc < 2 || vm.count("help")) {
			std::stringstream ss;
			ss << required_desc << std::endl;
			ss << additional_desc << std::endl;
			ss << help_desc << std::endl;
			throw BadArgs(ss.str());
		}

		po::store(po::command_line_parser(argc, argv).options(cmdline_options).run(), vm);
		po::notify(vm);

		htGammaD = 0.0005 * sigma * sigma - 0.0059 * sigma + 0.04;
		htTauTraj = 0.0047 * sigma * sigma + 0.0676 * sigma + 0.4564;
		htTauMatch = 0.0171 * sigma * sigma + 0.4520 * sigma + 47.9294;
	} catch (const po::error& e) {
		throw BadArgs(e.what());
	}
}
