/**
 * @brief Parse command line arguments
 * @author Alexander Pletzer
 *
 * This software is provided with the hope that it will be 
 * useful. It comes with no warranty whatsoever. 
 * Please send bug reports to alexander@gokliya.net.
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <limits>

#ifndef CMD_LINE_ARG_PARSER_H
#define CMD_LINE_ARG_PARSER_H

/**
 * @brief Simple command line argument parser
 */
class CmdLineArgParser {

 public:

  /**
   * Constructor
   */
  CmdLineArgParser() {
    // always activate -h
    this->set("-h", false, "Print help.");
    this->footer = "\n";    
  }
  
  // Setters
  /**
   * Set command line argument
   * @param name
   * @param defaultVal default value
   * @param help help string
   */
  void set(const std::string& name, double defaultVal, 
           const std::string& help);
  void set(const std::string& name, int defaultVal, 
           const std::string& help);
  void set(const std::string& name, const std::string& defaultVal, 
           const std::string& help);
  void set(const std::string& name, bool defaultVal, 
           const std::string& help);  
  /**
   * Parse command line arguments
   * @param argc number of arguments
   * @param argv arguments
   * @return true if all arguments were sucessfully parsed, false otherwise
   */
  bool parse(int argc, char *argv[]);

  /**
   * Set purpose of executable
   * @param purpose brief one-liner
   */
  void setPurpose(const std::string& purpose) {
    this->purpose = purpose;
  }

  /**
   * Print help to stdout
   */
  void help() const {
    std::cout << this->purpose << std::endl;
    std::cout << this->execName << " [options]\n";
    std::cout << "Usage:\n";
    for (std::map<std::string, double>::const_iterator
         i = this->doubleArg.begin();
       i != this->doubleArg.end(); ++i) {
      std::string hlp = this->doubleArgHelp.find(i->first)->second;
      std::cout << "\t" << i->first << " <double#> " << hlp
              << " (" << i->second << ")\n";
    }
    for (std::map<std::string, int>::const_iterator
         i = this->intArg.begin();
       i != this->intArg.end(); ++i) {
      std::string hlp = this->intArgHelp.find(i->first)->second;
      std::cout << "\t" << i->first << " <int#> " << hlp
              << " (" << i->second << ")\n";
    }
    for (std::map<std::string, std::string>::const_iterator
         i = this->stringArg.begin();
       i != this->stringArg.end(); ++i) {
      std::string hlp = this->stringArgHelp.find(i->first)->second;
      std::cout << "\t" << i->first << " <string> " << hlp
              << " (" << i->second << ")\n";
    }
    for (std::map<std::string, bool>::const_iterator
         i = this->boolArg.begin();
       i != this->boolArg.end(); ++i) {
      std::string hlp = this->boolArgHelp.find(i->first)->second;
      std::cout << "\t" << i->first << ' ' << hlp
              << " (" << i->second << ")\n";
    }
    std::cout << this->footer << std::endl;    
  }

  /**
   * Add, optional text to the help message
   * @param note text to add
   */
  void addFootnote(const std::string& note) {
    this->footer = note + "\n" + this->footer;
  }

  // Accessors
  
  /**
   * Get parameter value by name
   * @param name
   * @return value
   */
  template <class T>
  T get(const std::string& name) const;

 private:
  
  /** containers for parameters */
  std::map<std::string, double> doubleArg;
  std::map<std::string, int> intArg;
  std::map<std::string, std::string> stringArg;
  std::map<std::string, bool> boolArg;

  /** help strings */
  std::map<std::string, std::string> doubleArgHelp;
  std::map<std::string, std::string> intArgHelp;
  std::map<std::string, std::string> stringArgHelp;
  std::map<std::string, std::string> boolArgHelp;

  /** name of executable */
  std::string execName;

  /** brief one-liner describing executable */
  std::string purpose;

  /** footnote for help */
  std::string footer;
};

#endif // CMD_LINE_ARG_PARSER_H
