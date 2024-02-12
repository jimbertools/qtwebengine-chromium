#define JIMBERENABLED
#pragma once

#include <string>
namespace jimber {
class QuerySelector;
}

namespace blink {
class Document;
class Node;
class ExecutionContext;
class KURL;

class JTools {
 public:
  static JTools& instance() {
    static JTools instance;
    return instance;
  }

 private:
  JTools();
  std::vector<std::string> _sentFonts;
  std::vector<std::string> tagsBlackList{
      "SCRIPT", "BASE",
      "META"};  // no attributes and no text nodes, maybe change this to a map
                // e.g. script: "#text", base: "*", meta: "*"

  std::vector<std::string> attributeBlackList{
      // rewrite this logic with externalUrlList stuff
      "on.*",    "src",      "action",  "background", "cite",
      "classid", "codebase", "data",    "longdesc",   "profile",
      "usemap",  "srcdoc",   "sandbox", "integrity"};

  std::vector<std::string> externalUrlList{"href", "src", "poster", "srcset"};

  std::string m_userId;

 public:
  JTools(JTools const&) = delete;
  void operator=(JTools const&) = delete;
  void fixCSS(std::string& toReplace);
  void replaceFontDisplayOptional(std::string& toReplace);
  void replaceFontFamily(std::string& styleSheet);
  void replaceHover(std::string& toReplace);
  void replaceSrcSetURLs(std::string& toReplace, Document* document);

  std::string jReplaceAll(std::string& str,
                          const std::string& from,
                          const std::string& to);
  std::vector<std::string> splitCustomString(std::string stringToBeSplitted,
                                             std::string delimeter);
  bool hasParsingParent(Document* document);
  int getTagIndexInParent(Node* node);
  bool isForbiddenDomAction(const Node* parent, const Node* child);
  bool isBlackListedAttribute(const std::string& nodeName,
                              const std::string& attributeName,
                              const std::string& attributeValue);
  bool needsExternalResourceUrl(const std::string& attributeName);
  std::string replaceAllURLs(const Document* document, std::string decodedText);
  std::string replaceAllURLs(const KURL& baseURL, std::string styleSheet);
  std::string generateExternalResourceUrl(const KURL& baseURL,
                                          const std::string& url);
  std::string CreateCanonicalUUIDString();
  bool isSentFont(const std::string& fontName);
  void addSentFont(const std::string& fontName);
  static int getWindowIdForNode(const Node* node);
  static int getWindowIdForExecutionContext(const ExecutionContext* context);
};
}  // namespace blink
