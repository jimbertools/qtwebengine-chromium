#include <iostream>
#include <regex>

#include "JCommandHandler.h"
#include "JCommandTypes.h"
#include "JStringify.h"
#include "JTools.h"
#include "base/base64.h"
#include "base/hash/sha1.h"
#include "base/strings/string_number_conversions.h"
#include "third_party/blink/renderer/core/css/font_face.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/node.h"
#include "third_party/blink/renderer/core/dom/shadow_root.h"
#include "third_party/blink/renderer/core/dom/text.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_view.h"
#include "third_party/blink/renderer/core/html/forms/text_control_element.h"
#include "third_party/blink/renderer/core/html/html_iframe_element.h"
#include "third_party/blink/renderer/core/scroll/scrollable_area.h"
#include "third_party/blink/renderer/platform/graphics/image.h"

namespace blink {

JCommandHandler::JCommandHandler() {
  if (const char* env_p = std::getenv("DOMRENDERING_DISABLED")) {
    std::string env(env_p);
    if (env == "true") {
      m_rendererDisabled = true;
    }
  }
}

int JCommandHandler::m_currentWinIdInQueue = -1;
void JCommandHandler::handleQueue() {
  JCommandHandler::instance().mtx.lock();
  if (JCommandHandler::instance().m_queue.commands_size() > 0) {
    int size = JCommandHandler::instance().m_queue.ByteSize();
    char* data = new char[size];
    JCommandHandler::instance().m_queue.SerializeToArray(data, size);
    JCommandHandler::instance().m_queue = jimber::CommandQueue();
    const char* sizeOfValue = reinterpret_cast<char*>(&size);
    const char* windowId = reinterpret_cast<char*>(&m_currentWinIdInQueue);

    char* completeData = new char[(sizeof(int) * 2) + size];
    int* completeDataInt = reinterpret_cast<int*>(completeData);

    completeDataInt[0] = m_currentWinIdInQueue;
    completeDataInt[1] = size;

    // jelle
    for (size_t i = 0; i < size; i++) {
      completeData[i + 8] = data[i];
    }
    // alex
    // for (size_t i = 8; i < size; i++) {
    //   completeData[i] = data[i];
    // }

    // completeData =
    //     (std::string(sizeOfValue, sizeof(int)) + windowId + data).c_str();
    // // WindowId for the data
    // JCommandHandler::instance().handleNewCommand(windowId, sizeof(int));

    // // Size of the data which will come
    // JCommandHandler::instance().handleNewCommand(sizeOfValue, sizeof(int));
    // Actual data
    JCommandHandler::instance().handleNewCommand(completeData,
                                                 size + (sizeof(int) * 2));
    delete completeData;
    delete data;
  }
  JCommandHandler::instance().mtx.unlock();
}

void JCommandHandler::handleQueueRepeat() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    handleQueue();
  }
}

void JCommandHandler::sendCommand(int windowId,
                                  const jimber::Command& command) {
  if (windowId == -1) {
    // std::cout
    //     << "[JCommandHandler::sendCommand] not sending because winid is -1: "
    //     << command.DebugString() << std::endl;
    return;
  }

  if (m_rendererDisabled) {
    return;
  }

  if (!m_handleQueueThread.joinable()) {
    m_handleQueueThread = std::thread(handleQueueRepeat);
  }

  if (m_currentWinIdInQueue != windowId) {
    handleQueue();
  }

  JCommandHandler::instance().mtx.lock();
  jimber::Command* c = m_queue.add_commands();
  *c = command;
  JCommandHandler::instance().mtx.unlock();

  m_currentWinIdInQueue = windowId;
}

void JCommandHandler::sendDom(Document* document) {
  std::cout << "JCommandHandler::sendDom " << getpid() << std::endl;
  jimber::Command command;

  jimber::DomCommand* domCommand = new jimber::DomCommand();

  domCommand->set_nodeid(document->id());

  command.set_allocated_domcommand(domCommand);
  command.set_commandtype(jimber::CommandType::DOM);

  sendCommand(JTools::getWindowIdForNode(document), command);
  std::cout << "DOM SENT " << JTools::getWindowIdForNode(document) << std::endl;
  // if (JTools::getWindowIdForNode(document) == -1) {
  //   exit(0);
  // }
}

void JCommandHandler::sendInsertBefore(Node* parent,
                                       Node* refChild,
                                       Node* newChild) {
  // std::cout << "[JCommandHandler::sendInsertBefore]" << std::endl;

  jimber::Command command;
  jimber::InsertBeforeCommand* insertBeforeCommand =
      new jimber::InsertBeforeCommand();

  insertBeforeCommand->set_parentid(parent->id());
  insertBeforeCommand->set_refchildid(refChild->id());
  insertBeforeCommand->set_newchildid(newChild->id());

  command.set_allocated_insertbeforecommand(insertBeforeCommand);
  command.set_commandtype(jimber::CommandType::INSERTCHILD);
  sendCommand(JTools::getWindowIdForNode(refChild), command);
}

void JCommandHandler::sendReplaceChild(Node* toReplace, Node* replaceWith) {
  jimber::Command command;
  jimber::ReplaceChildCommand* replaceChildCommand =
      new jimber::ReplaceChildCommand();
  replaceChildCommand->set_toreplaceid(toReplace->id());
  replaceChildCommand->set_replacewithid(replaceWith->id());

  command.set_allocated_replacechildcommand(replaceChildCommand);
  command.set_commandtype(jimber::CommandType::REPLACECHILD);
  sendCommand(JTools::getWindowIdForNode(toReplace), command);
}

void JCommandHandler::sendRemoveChild(Node* toRemove) {
  jimber::Command command;
  jimber::RemoveChildCommand* removeChildCommand =
      new jimber::RemoveChildCommand();
  removeChildCommand->set_nodeid(toRemove->id());

  command.set_allocated_removechildcommand(removeChildCommand);
  command.set_commandtype(jimber::CommandType::REMOVECHILD);
  sendCommand(JTools::getWindowIdForNode(toRemove), command);
}

void JCommandHandler::sendCssTextUpdate(Element* element,
                                        std::string& cssText) {
  // std::cout << "JCommandHandler::sendCssTextUpdate " << element << " " <<
  // cssText << std::endl;
  jimber::Command command;
  jimber::CssTextUpdateCommand* cssTextUpdateCommand =
      new jimber::CssTextUpdateCommand();

  cssTextUpdateCommand->set_nodeid(element->id());

  JTools::instance().fixCSS(cssText);
  cssTextUpdateCommand->set_csstext(
      JTools::instance().replaceAllURLs(&element->GetDocument(), cssText));

  command.set_allocated_csstextupdatecommand(cssTextUpdateCommand);
  command.set_commandtype(jimber::CommandType::CSSTEXTUPDATE);
  sendCommand(JTools::getWindowIdForNode(element), command);
}

void JCommandHandler::sendCssPropertyUpdate(Element* element,
                                            const std::string& propertyName,
                                            const std::string& propertyValue) {
  // std::cout << "JCommandHandler::sendCssPropertyUpdate " << element << " " <<
  // propertyName << " " << propertyValue << std::endl;

  // bbc.com has nullptr here somewhre
  // hln too
  if (!element) {
    // std::cout << "Element is nullptr, so something went wrong. " << std::endl
    // << std::flush; //Research this
    return;
  }
  jimber::Command command;
  jimber::CssPropertyUpdateCommand* cssPropertyUpdateCommand =
      new jimber::CssPropertyUpdateCommand();
  cssPropertyUpdateCommand->set_nodeid(element->id());

  cssPropertyUpdateCommand->set_propertyname(propertyName);
  auto filteredValue =
      JTools::instance().replaceAllURLs(&element->GetDocument(), propertyValue);
  JTools::instance().fixCSS(filteredValue);

  cssPropertyUpdateCommand->set_propertyvalue(filteredValue);

  command.set_allocated_csspropertyupdatecommand(cssPropertyUpdateCommand);
  command.set_commandtype(jimber::CommandType::CSSPROPERTYUPDATE);
  sendCommand(JTools::getWindowIdForNode(element), command);
}

void JCommandHandler::sendSetCharacterData(Node* node,
                                           const std::string& value) {
  jimber::Command command;
  jimber::SetCharacterDataCommand* setCharacterDataCommand =
      new jimber::SetCharacterDataCommand();

  setCharacterDataCommand->set_nodeid(node->id());

  setCharacterDataCommand->set_data(value);

  command.set_allocated_setcharacterdatacommand(setCharacterDataCommand);
  command.set_commandtype(jimber::CommandType::SETCHARACTERDATA);
  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendInsertCssRule(Node* node,
                                        const std::string& rule,
                                        int ruleIndex,
                                        const std::string& uuid) {
  jimber::Command command;
  jimber::InsertCssRuleCommand* insertCssRuleCommand =
      new jimber::InsertCssRuleCommand();

  insertCssRuleCommand->set_constructeduuid(uuid);

  auto filteredValue =
      JTools::instance().replaceAllURLs(&node->GetDocument(), rule);
  JTools::instance().fixCSS(filteredValue);

  insertCssRuleCommand->set_rule(filteredValue);
  insertCssRuleCommand->set_ruleindex(ruleIndex);
  insertCssRuleCommand->set_nodeid(node->id());

  command.set_allocated_insertcssrulecommand(insertCssRuleCommand);
  command.set_commandtype(jimber::CommandType::INSERTCSSRULE);

  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendDeleteCssRule(Node* node,
                                        int ruleIndex,
                                        const std::string& uuid) {
  jimber::Command command;
  jimber::DeleteCssRuleCommand* deleteCssRuleCommand =
      new jimber::DeleteCssRuleCommand();

  deleteCssRuleCommand->set_constructeduuid(uuid);
  deleteCssRuleCommand->set_ruleindex(ruleIndex);

  deleteCssRuleCommand->set_nodeid(node->id());

  command.set_allocated_deletecssrulecommand(deleteCssRuleCommand);
  command.set_commandtype(jimber::CommandType::DELETECSSRULE);

  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendSetScroll(Node* node) {
  jimber::Command command;
  jimber::SetScrollCommand* setScrollCommand = new jimber::SetScrollCommand();
  int x = 0;
  int y = 0;
  if (node->IsElementNode()) {
    auto* element = DynamicTo<Element>(node);
    x = element->scrollLeft();
    y = element->scrollTop();
  }
  if (node->IsDocumentNode()) {
    auto* document = DynamicTo<Document>(node);
    x = document->View()->GetScrollableArea()->GetScrollOffset().Width();
    y = document->View()->GetScrollableArea()->GetScrollOffset().Height();
  }

  setScrollCommand->set_scrollx(x);
  setScrollCommand->set_scrolly(y);

  setScrollCommand->set_nodeid(node->id());

  command.set_allocated_setscrollcommand(setScrollCommand);
  command.set_commandtype(jimber::CommandType::SETSCROLL);

  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendSetFocus(Node* node, bool flag) {
  jimber::Command command;
  jimber::SetFocusCommand* setFocusCommand = new jimber::SetFocusCommand();

  setFocusCommand->set_nodeid(node->id());

  setFocusCommand->set_flag(flag);

  command.set_allocated_setfocuscommand(setFocusCommand);
  command.set_commandtype(jimber::CommandType::SETFOCUS);

  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendSetInputSelection(Node* node,
                                            unsigned start,
                                            unsigned end) {
  jimber::Command command;
  jimber::SetInputSelectionCommand* setInputSelectionCommand =
      new jimber::SetInputSelectionCommand();

  setInputSelectionCommand->set_nodeid(node->id());

  setInputSelectionCommand->set_start(start);
  setInputSelectionCommand->set_end(end);

  command.set_allocated_setinputselectioncommand(setInputSelectionCommand);
  command.set_commandtype(jimber::CommandType::SETINPUTSELECTION);

  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendSetValue(Node* node, const std::string& value) {
  jimber::Command command;
  jimber::SetValueCommand* setValueCommand = new jimber::SetValueCommand();

  setValueCommand->set_nodeid(node->id());
  setValueCommand->set_value(value);

  command.set_allocated_setvaluecommand(setValueCommand);
  command.set_commandtype(jimber::CommandType::SETVALUE);

  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendInsertTextIntoNode(Node* node,
                                             int offset,
                                             const std::string& text) {
  if (!node)  // not doing this crashes chatting in LinkedIn jelle 19/07/2021
    return;   // gmail too! alex 23/07/2021
  jimber::Command command;
  jimber::InsertTextIntoNodeCommand* insertTextIntoNodeCommand =
      new jimber::InsertTextIntoNodeCommand();

  insertTextIntoNodeCommand->set_nodeid(node->id());
  insertTextIntoNodeCommand->set_offset(offset);
  insertTextIntoNodeCommand->set_text(text);

  command.set_allocated_inserttextintonodecommand(insertTextIntoNodeCommand);
  command.set_commandtype(jimber::CommandType::INSERTTEXTINTONODE);

  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendDeleteTextFromNode(Node* node,
                                             int offset,
                                             int count) {
  jimber::Command command;
  jimber::DeleteTextFromNodeCommand* deleteTextFromNodeCommand =
      new jimber::DeleteTextFromNodeCommand();

  deleteTextFromNodeCommand->set_nodeid(node->id());

  deleteTextFromNodeCommand->set_offset(offset);
  deleteTextFromNodeCommand->set_count(count);

  command.set_allocated_deletetextfromnodecommand(deleteTextFromNodeCommand);
  command.set_commandtype(jimber::CommandType::DELETETEXTFROMNODE);

  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendReplaceTextInNode(Node* node,
                                            int offset,
                                            int count,
                                            const std::string& text) {
  jimber::Command command;
  jimber::ReplaceTextInNodeCommand* replaceTextInNodeCommand =
      new jimber::ReplaceTextInNodeCommand();

  replaceTextInNodeCommand->set_nodeid(node->id());

  replaceTextInNodeCommand->set_offset(offset);
  replaceTextInNodeCommand->set_count(count);
  replaceTextInNodeCommand->set_text(text);

  command.set_allocated_replacetextinnodecommand(replaceTextInNodeCommand);
  command.set_commandtype(jimber::CommandType::REPLACETEXTINNODE);

  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendSetSelection(Document* document,
                                       Node* startNode,
                                       int startOffset,
                                       int startType,
                                       Node* endNode,
                                       int endOffset,
                                       int endType) {
  jimber::Command command;
  jimber::SetSelectionCommand* setSelectionCommand =
      new jimber::SetSelectionCommand();

  if (startNode) {
    // We do this because a selection should be from element to element, but the
    // frontend wont select anything if this element is an html element
    if (startNode->nodeName() == "HTML") {
      startNode = document->body()->firstChild();
    }
    setSelectionCommand->set_startnodeid(startNode->id());
  }
  setSelectionCommand->set_startoffset(startOffset);
  setSelectionCommand->set_starttype(jimber::AnchorType(startType));

  if (endNode) {
    // We do this because a selection should be from element to element, but the
    // frontend wont select anything if this element is an html element
    if (endNode->nodeName() == "HTML") {
      endNode = document->body()->lastChild();
    }
    setSelectionCommand->set_endnodeid(endNode->id());
  }

  setSelectionCommand->set_endoffset(endOffset);
  setSelectionCommand->set_endtype(jimber::AnchorType(endType));

  setSelectionCommand->set_ownerdocumentid(document->id());

  command.set_allocated_setselectioncommand(setSelectionCommand);
  command.set_commandtype(jimber::CommandType::SETSELECTION);

  sendCommand(JTools::getWindowIdForNode(document), command);
}

void JCommandHandler::sendSetHovered(Node* node, bool hovered) {
  jimber::Command command;
  jimber::SetHoveredCommand* setHoveredCommand =
      new jimber::SetHoveredCommand();

  setHoveredCommand->set_nodeid(node->id());
  setHoveredCommand->set_hovered(hovered);

  command.set_allocated_sethoveredcommand(setHoveredCommand);
  command.set_commandtype(jimber::CommandType::SETHOVERED);

  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendAttachShadow(Node* node, ShadowRoot* shadowRoot) {
  jimber::Command command;
  jimber::AttachShadowCommand* attachShadowCommand =
      new jimber::AttachShadowCommand();

  attachShadowCommand->set_nodeid(node->id());
  attachShadowCommand->set_shadowid(shadowRoot->id());

  command.set_allocated_attachshadowcommand(attachShadowCommand);
  command.set_commandtype(jimber::CommandType::ATTACHSHADOW);

  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendCreateFontFace(Document* document,
                                         const std::string& fontName,
                                         const std::string& fontFile,
                                         int fontStyle,
                                         int fontWeight,
                                         int fontStretch) {
  jimber::Command command;
  jimber::CreateFontFaceCommand* createFontFaceCommand =
      new jimber::CreateFontFaceCommand();

  createFontFaceCommand->set_nodeid(reinterpret_cast<Node*>(document)->id());

  createFontFaceCommand->set_fontname(fontName);
  createFontFaceCommand->set_fontfile(fontFile);
  createFontFaceCommand->set_fontstyle(fontStyle);
  createFontFaceCommand->set_fontweight(fontWeight);
  createFontFaceCommand->set_fontstretch(fontStretch);

  command.set_allocated_createfontfacecommand(createFontFaceCommand);
  command.set_commandtype(jimber::CommandType::CREATEFONTFACE);

  sendCommand(JTools::getWindowIdForNode(document), command);
}
void JCommandHandler::sendCreateFontFace(ExecutionContext* context,
                                         FontFace* fontFace,
                                         std::string source) {
  if (!context->IsDocument()) {
    return;
  }
  jimber::Command command;
  jimber::CreateFontFaceFromJSCommand* createCommand =
      new jimber::CreateFontFaceFromJSCommand();
  const std::string externalSource =
      JTools::instance().replaceAllURLs(context->BaseURL(), source);
  createCommand->set_fontfaceuuid(fontFace->font_uuid_);
  createCommand->set_fontfamily(fontFace->family().Utf8());
  createCommand->set_source(externalSource);
  createCommand->set_fontstyle(fontFace->style().Utf8());
  createCommand->set_fontweight(fontFace->weight().Utf8());
  createCommand->set_fontstretch(fontFace->stretch().Utf8());
  createCommand->set_fontdisplay(fontFace->display().Utf8());

  command.set_allocated_createfontfacefromjscommand(createCommand);
  command.set_commandtype(jimber::CommandType::CREATEFONTFACEFROMJS);

  sendCommand(JTools::getWindowIdForExecutionContext(context), command);
}

void JCommandHandler::sendSetCssImage(const Document* document,
                                      const std::string& uuid) {
  jimber::Command command;
  jimber::SetCssImageCommand* setCssImageCommand =
      new jimber::SetCssImageCommand();
  setCssImageCommand->set_nodeid(reinterpret_cast<const Node*>(document)->id());

  setCssImageCommand->set_imageuuid(uuid);

  command.set_allocated_setcssimagecommand(setCssImageCommand);
  command.set_commandtype(jimber::CommandType::SETCSSIMAGE);

  sendCommand(JTools::getWindowIdForNode(document), command);
}

void JCommandHandler::sendReplaceCustomFont(const Document* document,
                                            const std::string& fontName) {
  jimber::Command command;
  jimber::ReplaceCustomFontCommand* replaceCustomFontCommand =
      new jimber::ReplaceCustomFontCommand();

  replaceCustomFontCommand->set_nodeid(
      reinterpret_cast<const Node*>(document)->id());

  replaceCustomFontCommand->set_fontname(fontName);

  command.set_allocated_replacecustomfontcommand(replaceCustomFontCommand);
  command.set_commandtype(jimber::CommandType::REPLACECUSTOMFONT);

  sendCommand(JTools::getWindowIdForNode(document), command);
}

void JCommandHandler::sendAddSourceBuffer(int winId,
                                          const std::string& mediaSourceId,
                                          const std::string& id,
                                          const std::string& type,
                                          const std::string& codecs) {
  jimber::Command command;
  auto addSourceBufferCommand = new jimber::AddSourceBufferCommand();
  addSourceBufferCommand->set_mediasourceid(mediaSourceId);
  addSourceBufferCommand->set_uuid(id);
  addSourceBufferCommand->set_mediatype(type);
  addSourceBufferCommand->set_codecs(codecs);
  command.set_allocated_addsourcebuffercommand(addSourceBufferCommand);
  command.set_commandtype(jimber::CommandType::ADDSOURCEBUFFER);
  sendCommand(winId, command);
}

void JCommandHandler::sendAppendDataToSourceBuffer(
    int winId,
    const std::string& mediaSourceId,
    const std::string& id,
    const double timestampOffset,
    const unsigned char* data,
    unsigned length) {
  jimber::Command command;
  auto msg = new jimber::AppendDataToSourceBufferCommand();
  msg->set_uuid(id);
  msg->set_mediasourceid(mediaSourceId);
  msg->set_data(data, length);
  command.set_allocated_appenddatatosourcebuffercommand(msg);
  command.set_commandtype(jimber::CommandType::APPENDDATATOSOURCEBUFFER);
  sendCommand(winId, command);
}

void JCommandHandler::sendAttachMediaSourceToElement(Element* element,
                                                     const std::string& id) {
  jimber::Command command;
  auto msg = new jimber::AttachMediaSourceToElementCommand();
  msg->set_nodeid(element->id());
  msg->set_mediasourceid(id);
  command.set_allocated_attachmediasourcetoelementcommand(msg);
  command.set_commandtype(jimber::CommandType::ATTACHMEDIASOURCETOELEMENT);
  sendCommand(JTools::getWindowIdForNode(element), command);
}

void JCommandHandler::sendMediaElementAction(Element* element,
                                             jimber::MediaElementAction action,
                                             double value) {
  jimber::Command command;
  auto msg = new jimber::MediaElementActionCommand();
  msg->set_nodeid(element->id());

  msg->set_action(action);
  msg->set_value(value);
  command.set_allocated_mediaelementactioncommand(msg);
  command.set_commandtype(jimber::CommandType::MEDIAELEMENTACTION);
  sendCommand(JTools::getWindowIdForNode(element), command);
}

void JCommandHandler::sendMediaSourceAction(int winId,
                                            const std::string& id,
                                            jimber::MediaSourceAction action) {
  jimber::Command command;
  auto msg = new jimber::MediaSourceActionCommand();
  msg->set_uuid(id);
  msg->set_action(action);
  command.set_allocated_mediasourceactioncommand(msg);
  command.set_commandtype(jimber::CommandType::MEDIASOURCEACTION);
  sendCommand(winId, command);
}

// void JCommandHandler::sendSetAppendWindowStartCommand(const std::string&
// id,
//                                                       const double time);
// void JCommandHandler::sendSetAppendWindowEndCommand(const std::string& id,
//                                                     const double time);

void JCommandHandler::sendResetSourceBufferParser(
    int winId,
    const std::string& mediaSourceId,
    const std::string& id) {
  jimber::Command command;
  auto msg = new jimber::ResetSourceBufferParserCommand();
  msg->set_uuid(id);
  msg->set_mediasourceid(mediaSourceId);
  command.set_allocated_resetsourcebufferparsercommand(msg);
  command.set_commandtype(jimber::CommandType::RESETSOURCEBUFFERPARSER);
  sendCommand(winId, command);
}

void JCommandHandler::sendCreateNode(Node* node) {
  int winId = JTools::instance().getWindowIdForNode(node);
  jimber::Command command;
  auto msg = new jimber::CreateNodeCommand();
  msg->set_nodename(node->nodeName().Utf8());
  if (node->IsElementNode()) {
    msg->set_nodetype(jimber::NodeType::ELEMENT);
    msg->set_documentnodeid(node->GetDocument().id());
    if (node->IsSVGElement()) {
      Element* element = reinterpret_cast<Element*>(node);
      std::string ns =
          element->TagQName().NamespaceURI() == "http://www.w3.org/1999/xhtml"
              ? ""
              : element->TagQName().NamespaceURI().Utf8();
      msg->set_namespace_(ns);
    }
  }
  if (node->IsDocumentNode()) {
    if (node->GetDocument().ContextDocument()) {
      msg->set_ownerdocumentnodeid(node->GetDocument().ContextDocument()->id());
    }
    msg->set_nodetype(jimber::NodeType::DOCUMENT);
  }
  if (node->IsDocumentFragment()) {
    msg->set_nodetype(jimber::NodeType::FRAGMENT);
  }
  if (node->IsTextNode()) {
    msg->set_nodetype(jimber::NodeType::TEXT);
  }
  if (node->getNodeType() == Node::kCommentNode) {
    msg->set_nodetype(jimber::NodeType::COMMENT);
  }
  if (node->IsDocumentTypeNode()) {
    msg->set_nodetype(jimber::NodeType::DOCTYPE);
  }
  if (node->IsShadowRoot()) {
    msg->set_nodetype(jimber::NodeType::SHADOW);
  }

  msg->set_nodeid(node->id());
  auto* localOwner = node->GetDocument().LocalOwner();
  if (localOwner) {
    msg->set_localownerid(localOwner->id());
  }

  if (node->IsCharacterDataNode()) {
    auto* characterData = DynamicTo<CharacterData>(node);
    msg->set_nodevalue(characterData->data().Utf8());
  }

  command.set_allocated_createnodecommand(msg);
  command.set_commandtype(jimber::CommandType::CREATENODE);

  sendCommand(winId, command);
}

void JCommandHandler::sendDeleteNode(Node* node) {
  return;  // This causes the browser to crash. (getWindowIdForNode)
  std::cout << "[JCommandHandler::sendDeleteNode] " << std::endl;
  int winId = JTools::instance().getWindowIdForNode(node);
  std::cout << "gojemagoweiphalenvintje" << std::endl;
  // temp
  jimber::Command command;
  auto msg = new jimber::DeleteNodeCommand();
  msg->set_nodeid(node->id());
  command.set_allocated_deletenodecommand(msg);
  command.set_commandtype(jimber::CommandType::DELETENODE);

  sendCommand(winId, command);
  //

  // std::cout << "[JCommandHandler::sendDeleteNode] " << node << std::endl;

  // auto context = node->GetExecutionContext();
  // if (!context)
  //   return;
  // std::cout << "[JCommandHandler::sendDeleteNode] has context " << std::endl;
  // auto window = context->ExecutingWindow();
  // if (!window)
  //   return;
  // std::cout << "[JCommandHandler::sendDeleteNode] has window " << std::endl;
  // auto frame = window->GetFrame();
  // if (!frame)
  //   return;

  // std::cout << "[JCommandHandler::sendDeleteNode] has frame" << std::endl;
  // std::cout << "[JCommandHandler::sendDeleteNode] getting winid" <<
  // std::endl; int winId = JTools::getWindowIdForNode(node);
  // std::cout << "[JCommandHandler::sendDeleteNode] got it " << winId
  //           << std::endl;

  // jimber::Command command;
  // auto msg = new jimber::DeleteNodeCommand();
  // msg->set_nodeid(node->id());
  // command.set_allocated_deletenodecommand(msg);
  // command.set_commandtype(jimber::CommandType::DELETENODE);

  // sendCommand(winId, command);
}

void JCommandHandler::sendAppendChild(Node* parentNode, Node* newChild) {
  jimber::Command command;
  auto msg = new jimber::AppendChildCommand();
  msg->set_parentid(parentNode->id());
  msg->set_childid(newChild->id());
  command.set_allocated_appendchildcommand(msg);
  command.set_commandtype(jimber::CommandType::APPENDCHILD);
  sendCommand(JTools::getWindowIdForNode(parentNode), command);
}

void JCommandHandler::sendSetAttribute(Node* node,
                                       const std::string& attributeName,
                                       const std::string& attributeValue,
                                       bool null) {
  // std::cout << "JCommandHandler::sendSetAttribute " << node << " " <<
  // attributeName << " " << attributeValue << std::endl;

  jimber::Command command;
  auto msg = new jimber::AttributeChangedCommand();
  msg->set_nodeid(node->id());

  msg->set_attributename(attributeName);

  std::string value = attributeValue;
  if (JTools::instance().needsExternalResourceUrl(attributeName) &&
      !(node->nodeName() == "use" && attributeName == "href" &&
        attributeValue.rfind("#", 0) == 0)) {
    if (attributeName == "srcset") {
      JTools::instance().replaceSrcSetURLs(value, &node->GetDocument());
    } else {
      blink::KURL url = KURL(value.c_str());
      if (!url.ProtocolIsData()) {
        value = JTools::instance().generateExternalResourceUrl(
            node->GetDocument().BaseURL(), value);
      }
    }
  }

  msg->set_attributevalue(value);
  msg->set_null(null);

  command.set_allocated_attributechangedcommand(msg);
  command.set_commandtype(jimber::CommandType::ATTRIBUTECHANGED);
  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendCreateConstructedStyleSheet(
    const Document* document,
    const std::string& constructedUUID) {
  jimber::Command command;
  auto msg = new jimber::CreateConstructedStyleSheetCommand();
  msg->set_nodeid(document->id());
  msg->set_constructeduuid(constructedUUID);

  command.set_allocated_createconstructedstylesheetcommand(msg);
  command.set_commandtype(jimber::CommandType::CREATECONSTRUCTEDSTYLESHEET);
  sendCommand(JTools::getWindowIdForNode(document), command);
}

void JCommandHandler::sendSetAdoptedStyleSheets(
    const Node* node,
    const HeapVector<Member<CSSStyleSheet>>& adopted_style_sheets) {
  jimber::Command command;
  auto msg = new jimber::SetAdoptedStyleSheetsCommand();
  msg->set_nodeid(node->id());

  for (CSSStyleSheet* sheet : adopted_style_sheets) {
    auto constructed = msg->add_constructeduuid();
    *constructed = sheet->uuid();
  }

  command.set_allocated_setadoptedstylesheetscommand(msg);
  command.set_commandtype(jimber::CommandType::SETADOPTEDSTYLESHEETS);
  sendCommand(JTools::getWindowIdForNode(node), command);
}

void JCommandHandler::sendCSSStyleSheetReplace(
    const Document* document,
    const std::string& constructedUUID,
    const std::string& replaceString,
    bool sync) {
  jimber::Command command;
  auto msg = new jimber::CSSStyleSheetReplaceCommand();
  msg->set_nodeid(document->id());
  msg->set_constructeduuid(constructedUUID);

  auto filteredValue =
      JTools::instance().replaceAllURLs(document, replaceString);
  JTools::instance().fixCSS(filteredValue);
  msg->set_replacestring(filteredValue);
  msg->set_sync(sync);

  command.set_allocated_cssstylesheetreplacecommand(msg);
  command.set_commandtype(jimber::CommandType::CSSSTYLESHEETREPLACE);
  sendCommand(JTools::getWindowIdForNode(document), command);
}

void JCommandHandler::sendClearDocument(const Document* document) {
  jimber::Command command;
  auto msg = new jimber::ClearDocumentCommand();
  msg->set_nodeid(document->id());

  command.set_allocated_cleardocumentcommand(msg);
  command.set_commandtype(jimber::CommandType::CLEARDOCUMENT);
  sendCommand(JTools::instance().getWindowIdForNode(document), command);
}

void JCommandHandler::sendSetChecked(const Node* node, bool checked) {
  jimber::Command command;
  auto msg = new jimber::SetCheckedCommand();
  msg->set_nodeid(node->id());
  msg->set_checked(checked);

  command.set_allocated_setcheckedcommand(msg);
  command.set_commandtype(jimber::CommandType::SETCHECKED);
  sendCommand(JTools::instance().getWindowIdForNode(node), command);
}

void JCommandHandler::sendOpenSelectionMenu(
    const Node* node,
    int x,
    int y,
    int width,
    int height,
    const std::vector<std::string>& options) {
  jimber::Command command;
  auto msg = new jimber::OpenSelectionMenuCommand();
  msg->set_nodeid(node->id());
  msg->set_x(x);
  msg->set_y(y);
  msg->set_width(width);
  msg->set_height(height);

  std::string* option;

  for (const std::string& value : options) {
    option = msg->add_options();
    *option = value;
  }

  command.set_allocated_openselectionmenucommand(msg);
  command.set_commandtype(jimber::CommandType::OPENSELECTIONMENU);
  sendCommand(JTools::instance().getWindowIdForNode(node), command);
}

void JCommandHandler::sendCloseSelectionMenu(const Node* node) {
  jimber::Command command;
  auto msg = new jimber::CloseSelectionMenuCommand();
  msg->set_nodeid(node->id());

  command.set_allocated_closeselectionmenucommand(msg);
  command.set_commandtype(jimber::CommandType::CLOSESELECTIONMENU);
  sendCommand(JTools::instance().getWindowIdForNode(node), command);
}

void JCommandHandler::sendSelectOption(Node* node, int index) {
  jimber::Command command;
  auto msg = new jimber::SetSelectedOptionCommand();
  msg->set_nodeid(node->id());
  msg->set_selectoptionindex(index);

  command.set_allocated_setselectedoptioncommand(msg);
  command.set_commandtype(jimber::CommandType::SETSELECTEDOPTION);
  sendCommand(JTools::instance().getWindowIdForNode(node), command);
}

// void JCommandHandler::sendCanvasVideoData(int winId,
//                                           int nodeId,
//                                           const std::string& encoded_data) {
//   if (winId == -1)
//     return;
//   jimber::Command command;
//   auto msg = new jimber::CanvasVideoDataCommand();
//   msg->set_nodeid(nodeId);
//   msg->set_data(encoded_data);

//   command.set_allocated_canvasvideodatacommand(msg);
//   command.set_commandtype(jimber::CommandType::CANVASVIDEODATA);
//   sendCommand(winId, command);
// }
void JCommandHandler::sendCanvasVideoData(const Node* node,
                                          int realWidth,
                                          int realHeight,
                                          const char* data,
                                          unsigned length) {
  std::cout << "[JCommandHandler::sendCanvasVideoData]" << std::endl;
  jimber::Command command;
  auto msg = new jimber::CanvasVideoDataCommand();
  msg->set_nodeid(node->id());
  msg->set_width(realWidth);
  msg->set_height(realHeight);
  msg->set_data(data, length);

  command.set_allocated_canvasvideodatacommand(msg);
  command.set_commandtype(jimber::CommandType::CANVASVIDEODATA);
  sendCommand(JTools::instance().getWindowIdForNode(node), command);
}

void JCommandHandler::sendCanvasImageData(const Node* node,
                                          int realWidth,
                                          int realHeight,
                                          const char* data,
                                          unsigned length) {
  // std::cout << "[JCommandHandler::sendCanvasImageData]" << std::endl;

  jimber::Command command;
  auto msg = new jimber::CanvasImageDataCommand();
  msg->set_nodeid(node->id());
  msg->set_width(realWidth);
  msg->set_height(realHeight);
  msg->set_data(data, length);

  command.set_allocated_canvasimagedatacommand(msg);
  command.set_commandtype(jimber::CommandType::CANVASIMAGEDATA);
  sendCommand(JTools::instance().getWindowIdForNode(node), command);
}
void JCommandHandler::sendClearCanvas(const Node* node) {
  jimber::Command command;
  auto msg = new jimber::ClearCanvasCommand();
  msg->set_nodeid(node->id());

  command.set_allocated_clearcanvascommand(msg);
  command.set_commandtype(jimber::CommandType::CLEARCANVAS);
  sendCommand(JTools::instance().getWindowIdForNode(node), command);
}

void JCommandHandler::sendImageWithBlobUrl(const Element* element,
                                           Image* image,
                                           const std::string blobUrl,
                                           const std::string mimeType) {
  const auto sharedBuffer = image->Data();

  jimber::Command command;
  auto msg = new jimber::ImageWithBlobUrlcommand();
  msg->set_nodeid(element->id());
  msg->set_data(sharedBuffer->Data(), sharedBuffer->size());
  msg->set_bloburl(blobUrl);
  msg->set_mimetype(mimeType);

  command.set_allocated_imagewithbloburlcommand(msg);
  command.set_commandtype(jimber::CommandType::IMAGEWITHBLOBURL);
  sendCommand(JTools::instance().getWindowIdForNode(element), command);
}
void JCommandHandler::sendFullScreenRequest(const Node* node) {
  jimber::Command command;
  auto msg = new jimber::FullScreenRequestCommand();
  msg->set_nodeid(node->id());

  command.set_allocated_fullscreenrequestcommand(msg);
  command.set_commandtype(jimber::CommandType::FULLSCREENREQUEST);
  sendCommand(JTools::instance().getWindowIdForNode(node), command);
}

void JCommandHandler::sendAttachFontFaceToDocument(const Document* document,
                                                   std::string fontFaceUUID) {
  jimber::Command command;
  auto msg = new jimber::AttachFontFaceToDocument();
  msg->set_nodeid(document->id());
  msg->set_fontfaceuuid(fontFaceUUID);
  command.set_allocated_attachfontfacetodocument(msg);
  command.set_commandtype(jimber::CommandType::ATTACHFONTFACETODOCUMENT);
  sendCommand(JTools::instance().getWindowIdForNode(document), command);
}

}  // namespace blink