#include "JTools.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include "base/base64url.h"
#include "base/guid.h"
#include "base/hash/sha1.h"
#include "base/strings/string_number_conversions.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/dom/flat_tree_traversal.h"
#include "third_party/blink/renderer/core/dom/node.h"
#include "third_party/blink/renderer/core/dom/node_traversal.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/html/html_iframe_element.h"
#include "third_party/blink/renderer/core/html/jimber/protobuf/cpp/domcommands.pb.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

#include "third_party/blink/renderer/platform/weborigin/kurl.h"

namespace blink {

JTools::JTools() {
  if (const char* env_p = std::getenv("USER_ID")) {
    m_userId = env_p;
  }
}

void JTools::fixCSS(std::string& toReplace) {
  replaceHover(toReplace);
  replaceFontDisplayOptional(toReplace);
  replaceFontFamily(toReplace);
}

// We don't really "hover" in the frontend and :hover cannot be triggered
// through Javascript. Because of this, we have to use the j-fake-hover class
// and apply it whenever a hover occurs in the backend.
void JTools::replaceHover(std::string& toReplace) {
  jReplaceAll(toReplace, ":hover", ".j-fake-hover");
}

// The system-ui font-family parameter will pick a system font. We have to
// replace this with j-system-ui so that it does not do this. If it would swap
// to a system font, it would pick a different font for every different
// operating system or environment.
void JTools::replaceFontFamily(std::string& styleSheet) {
  std::regex r("(font-family:([a-zA-Z, '\"-]*)?)(system-ui)");
  styleSheet = std::regex_replace(styleSheet, r, "$1j-system-ui");

  r = std::regex("(font-family:([a-zA-Z, '\"-]*)?)(initial)");
  styleSheet = std::regex_replace(styleSheet, r, "$1j-initial");

  r = std::regex("(font-family:([a-zA-Z, '\"-]*)?)(ui-sans-serif)");
  styleSheet = std::regex_replace(styleSheet, r, "$1j-ui-sans-serif");

  r = std::regex("(font-family:([a-zA-Z, '\"-]*)?)(-apple-system)");
  styleSheet = std::regex_replace(styleSheet, r, "$1j-apple-system");

  r = std::regex("(font-family:([a-zA-Z, '\"-]*)?)(BlinkMacSystemFont)");
  styleSheet = std::regex_replace(styleSheet, r, "$1j-BlinkMacSystemFont");
}

// This is needed because font-display optional will not "swap" the font after
// it's loaded. Therefor, if we create a fontface and later remove the fontface
// to use the custom font, it will not swap anymore.
void JTools::replaceFontDisplayOptional(std::string& styleSheet) {
  std::regex e("(font-display:([a-zA-Z, '\"-]*)?)(optional)");
  styleSheet = std::regex_replace(styleSheet, e, "$1auto");
}

void JTools::replaceSrcSetURLs(std::string& srcset, Document* document) {
  std::regex newlines_re("\n+");
  srcset = std::regex_replace(srcset, newlines_re, "");

  if (srcset.find(" ") == std::string::npos) {  // Sometimes a srcset attribute
                                                // contains just an URL...
    srcset = generateExternalResourceUrl(document->BaseURL(), srcset);
    return;
  }

  std::regex regularExpression(",?(.*?) (\\d+)(x|px|w|h)");

  std::map<std::string, std::string> urlMap;
  std::sregex_iterator begin(srcset.begin(), srcset.end(), regularExpression);
  std::sregex_iterator end = std::sregex_iterator();
  for (std::sregex_iterator i = begin; i != end; ++i) {
    std::smatch match = *i;

    std::string toReplace = match[1].str();
    blink::KURL url = KURL(toReplace.c_str());

    if (!url.ProtocolIsData() && !match[1].str().empty()) {
      urlMap[toReplace] = generateExternalResourceUrl(document->BaseURL(),
                                                      match[1].str().c_str());
    }
  }

  for (auto const& urlSetToReplace : urlMap) {
    srcset = jReplaceAll(srcset, urlSetToReplace.first, urlSetToReplace.second);
  }
}

bool JTools::isSentFont(const std::string& fontName) {
  return (std::find(_sentFonts.begin(), _sentFonts.end(), fontName) !=
          _sentFonts.end());
}

void JTools::addSentFont(const std::string& fontName) {
  _sentFonts.push_back(fontName);
}

std::vector<std::string> JTools::splitCustomString(
    std::string stringToBeSplitted,
    std::string delimeter) {
  std::vector<std::string> splittedString;
  int startIndex = 0;
  int endIndex = 0;
  while ((endIndex = stringToBeSplitted.find(delimeter, startIndex)) <
         stringToBeSplitted.size()) {
    std::string val =
        stringToBeSplitted.substr(startIndex, endIndex - startIndex);
    splittedString.push_back(val);
    startIndex = endIndex + delimeter.size();
  }
  if (startIndex < stringToBeSplitted.size()) {
    std::string val = stringToBeSplitted.substr(startIndex);
    splittedString.push_back(val);
  }
  return splittedString;
};

std::string JTools::jReplaceAll(std::string& str,
                                const std::string& from,
                                const std::string& to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos +=
        to.length();  // Handles case where 'to' is a substring of 'from'
  }
  return str;
}

int JTools::getTagIndexInParent(Node* node) {
  int childIndex = 1;

  for (Node& child : NodeTraversal::ChildrenOf(*node->parentNode())) {
    if (child.nodeName() == node->nodeName()) {
      if (child.NodeIndex() == node->NodeIndex()) {
        break;
      }
      childIndex++;
    }
  }

  return childIndex;
}

bool JTools::isForbiddenDomAction(const Node* parent, const Node* child) {
  if (!parent->IsHTMLElement() ||
      (child != nullptr &&
       child->IsHTMLElement()))  // all elements now allowed, only text nodes
                                 // etc are not
    return false;

  auto parentEl = DynamicTo<HTMLElement>(parent);

  bool blacklisted =
      std::find(tagsBlackList.begin(), tagsBlackList.end(),
                parentEl->tagName().Utf8()) != tagsBlackList.end();
  return blacklisted;
}

bool JTools::isBlackListedAttribute(const std::string& nodeName,
                                    const std::string& attributeName,
                                    const std::string& attributeValue) {
  if (nodeName == "LINK" && attributeName == "as" &&
      attributeValue == "script") {
    return true;
  }
  if (nodeName == "IMG" && attributeName == "src")
    return false;

  if ((nodeName == "VIDEO" || nodeName == "SOURCE") && attributeName == "src") {
    return false;
  }

  for (int i = 0; i < attributeBlackList.size(); i++) {
    std::regex regularExpression("^" + attributeBlackList[i] + "$");
    std::smatch match;
    std::regex_search(attributeName, match, regularExpression);
    if (!match.empty()) {
      return true;
    }
  }
  return false;
}

std::string JTools::CreateCanonicalUUIDString() {
  return base::GenerateGUID();
  // return String::FromUTF8(uuid.data(), uuid.length());
}

std::string JTools::replaceAllURLs(const Document* document,
                                   std::string styleSheet) {
  return replaceAllURLs(document->BaseURL(), styleSheet);
}

std::string JTools::replaceAllURLs(const KURL& baseURL,
                                   std::string styleSheet) {
  std::regex regularExpression(
      "(@import |@import "
      "url|url)\\(? ?('|\")?((?!data:)[a-zA-Z0-9\\?\\/"
      ":.\\-_#=& ~@]+?)('|\"|\\))");

  std::map<std::string, std::string> urlMap;
  std::sregex_iterator begin(styleSheet.begin(), styleSheet.end(),
                             regularExpression);
  std::sregex_iterator end = std::sregex_iterator();

  for (std::sregex_iterator i = begin; i != end; ++i) {
    std::smatch match = *i;
    std::string toReplace = match[3].str();

    urlMap[toReplace] =
        generateExternalResourceUrl(baseURL, match[3].str().c_str());
  }
  for (auto const& urlSetToReplace : urlMap) {
    styleSheet =
        jReplaceAll(styleSheet, urlSetToReplace.first, urlSetToReplace.second);
  }

  return styleSheet;
}

bool JTools::needsExternalResourceUrl(const std::string& attributeName) {
  return std::find(externalUrlList.begin(), externalUrlList.end(),
                   attributeName) != externalUrlList.end();
}

// Oh boy, this function used to be simple.
// There seems to be something wrong with the way
// document->CompleteURL handles "special" characters like（which is not ( !
// So what we did was take only the things we think we need and call them
// ourselves Neither of us is convinced this solves all cases but hey ¯\_(ツ)_/¯
// it seems to work
std::string JTools::generateExternalResourceUrl(const KURL& baseUrl,
                                                const std::string& url) {
  if (url.empty())
    return "";

  std::string urlPart = url;
  std::string hashPart = "";
  if (url.find("#") != std::string::npos) {
    std::vector<std::string> urlSplitArray = splitCustomString(url, "#");
    urlPart = urlSplitArray.at(0);

    hashPart = urlSplitArray.size() > 1 ? urlSplitArray.at(1) : "";
  }
  // urlPart.erase(remove_if(urlPart.begin(), urlPart.end(), isspace),
  // urlPart.end());

  // KURL baseUrl = document->baseURI();
  url::RawCanonOutputT<char> output;
  const char* rawBaseUrl =
      reinterpret_cast<const char*>(baseUrl.GetString().Impl()->Bytes());
  const char* rawUrlPart = reinterpret_cast<const char*>(urlPart.c_str());

  url::Parsed parsed;
  const url::Parsed baseParsed = baseUrl.GetParsed();
  url::ResolveRelative(rawBaseUrl, baseUrl.GetString().length(), baseParsed,
                       rawUrlPart, urlPart.length(), nullptr, &output, &parsed);
  const String absoluteUrlString =
      AtomicString::FromUTF8(output.data(), output.length());

  std::string b64Url;

  base::Base64UrlEncode(base::StringPiece(absoluteUrlString.Utf8()),
                        base::Base64UrlEncodePolicy::OMIT_PADDING, &b64Url);
  if (!hashPart.empty()) {
    b64Url = b64Url + "#" + hashPart;
  }

  if (m_userId.length()) {
    return "/resource/" + m_userId + "?url=" + b64Url;
  }

  return "/resource?url=" + b64Url + "&originalurl=" + urlPart;
}

int JTools::getWindowIdForNode(const Node* node) {
  // this makes things go missing on linkedin
  // if(!node->GetDocument().GetFrame()) return -1;
  return getWindowIdForExecutionContext(node->GetExecutionContext());
}

int JTools::getWindowIdForExecutionContext(const ExecutionContext* context) {
  if (!context)
    return -1;
  if (!context->ExecutingWindow())
    return -1;

  auto frame = context->ExecutingWindow()->GetFrame();

  if (!frame)
    return -1;

  int windowId = frame->windowId_;

  if (windowId != -1) {
    return windowId;
  }

  auto localOwner = frame->DeprecatedLocalOwner();

  // while (localOwner && localOwner->GetDocument().LocalOwner()) {
  //   localOwner = localOwner->GetDocument().LocalOwner();
  // }
  while (localOwner && localOwner->GetDocument().LocalOwner()) {
    localOwner = localOwner->GetDocument().LocalOwner();
  }

  if (localOwner) {
    windowId = localOwner->GetExecutionContext()
                   ->ExecutingWindow()
                   ->GetFrame()
                   ->windowId_;
  }

  if (windowId == -1) {
    // std::cout << "[JTools::getWindowIdForNode]" << node << std::endl;
    // exit(0);
  }
  return windowId;
}

}  // namespace blink