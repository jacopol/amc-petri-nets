#include "pnml.h"

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <utility>

/////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS AND PARSING                                                            //
/////////////////////////////////////////////////////////////////////////////////////////
pnml::pnml(std::istream &is)
{
  pugi::xml_document doc;

  const pugi::xml_parse_result result = doc.load(is);
  if (!result) { throw std::runtime_error("PNML file could not be parsed"); }

  parse_pnml(doc);
}

pnml::pnml(const std::filesystem::path &path)
{
  pugi::xml_document doc;

  const pugi::xml_parse_result result = doc.load_file(path.string().data());
  if (!result) { throw std::runtime_error("PNML file could not be parsed"); }

  parse_pnml(doc);
}

void
pnml::parse_pnml(pugi::xml_document &doc)
{
  std::unordered_map<std::string, int> transition_names_to_int;
  std::unordered_map<std::string, int> place_names_to_int;

  const auto convert = [&](const std::string &name) {
    const bool is_place   = place_names_to_int.find(name) != place_names_to_int.end();
    const bool is_transition = transition_names_to_int.find(name) != transition_names_to_int.end();

    if (!is_place && !is_transition) {
      throw std::runtime_error("Unknown place or transition '" + name + "'");
    }

    const id_type id = is_place
      ? place_names_to_int.find(name)->second
      : transition_names_to_int.find(name)->second;

    return std::pair<bool, id_type>(is_place, id);
  };

  const pugi::xml_node &doc_page = doc.child("pnml").child("net").child("page");
  for (const pugi::xml_node &n : doc_page) {
    const std::string n_name(n.name());

    if (n_name == "place") {
      const std::string name = n.attribute("id").value();

      const id_type place = this->_places++;

      assert(place == this->_place_names.size());
      this->_place_names.push_back(name);

      assert(place == this->_place_names_to_int.size());
      place_names_to_int.insert({ name, place });

      if (n.child("initialMarking")) {
        this->_place_initial.insert(place);
      }
    } else if (n_name == "transition") {
      const std::string name = n.attribute("id").value();

      const id_type transition = this->_transitions++;

      assert(transition == transition_names_to_int.size());
      transition_names_to_int.insert({ name, transition });

      assert(transition == transition_names_to_int.size());
      this->_transition_pre.push_back({});

      this->_transition_post.push_back({});
    } else if (n_name == "arc") {
      const std::string source_name = n.attribute("source").value();
      const auto [source_is_place, source_id] = convert(source_name);

      const std::string target_name = n.attribute("target").value();
      const auto [target_is_place, target_id] = convert(target_name);

      if (source_is_place == target_is_place) {
        throw std::runtime_error("Arc connects '" + source_name
                                 + "' to '" + target_name
                                 + "' of the same type");
      }

      if (source_is_place) {
        this->_transition_pre.at(target_id).insert(source_id);
      } else { // if (target_is_place)
        this->_transition_post.at(source_id).insert(target_id);
      }
    } else {
      // ignore node and continue...
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
// GETTERS                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////
pnml::size_type
pnml::places() const
{
  return this->_places;
}

const pnml::marking_type&
pnml::initial() const
{
  return this->_place_initial;
}

pnml::size_type
pnml::transitions() const
{
  return this->_transitions;
}

pnml::transition_type
pnml::transition(const id_type &t) const
{
  return { this->_transition_pre.at(t), this->_transition_post.at(t) };
}

/////////////////////////////////////////////////////////////////////////////////////////
// OTHER                                                                               //
/////////////////////////////////////////////////////////////////////////////////////////
std::string
pnml::to_string() const
{
  std::stringstream ss;

  // Print Places
  ss << "places:\n"
     << "  ";
  switch (this->places()) {
  case 0:
    ss << "Ø\n";
    break;
  case 1:
    ss << "0\n";
    break;
  case 2:
    ss << "0, 1\n";
    break;
  case 3:
    ss << "0, 1, 2\n";
    break;
  default:
    ss << "0, 1, ..., " << this->places()-1 << "\n";
  }
  ss << "\n";

  // Initial Marking
  ss << "initial marking:\n"
     << "  { ";
  for (const id_type l : this->initial()) {
    ss << l << " ";
  }
  ss << "}\n"
     << "\n";

  // Print transitions
  ss << "transitions:\n";

  for (id_type t = 0; t < this->transitions(); ++t) {
    const transition_type transition = this->transition(t);

    ss << "  { ";
    for (const id_type l : transition.first) {
      ss << l << " ";
    }
    ss << "} -> { ";
    for (const id_type l : transition.second) {
      ss << l << " ";
    }
    ss << "}\n";
  }

  return ss.str();
}
