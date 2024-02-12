#pragma once

#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include "third_party/blink/renderer/core/html/jimber/protobuf/cpp/domcommands.pb.h"
#include "third_party/blink/renderer/platform/heap/heap_allocator.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

typedef std::function<void(std::string&)> HandlerMethod;
typedef std::function<void(const char*, int)> newHandlerMethod;

namespace blink {
class Document;
class Image;
class Node;
class Element;
class ShadowRoot;
class CSSStyleSheet;
class FontFace;
class ExecutionContext;

class JCommandHandler {
 public:
  static JCommandHandler& instance() {
    static JCommandHandler instance;

    return instance;
  }

 private:
  JCommandHandler();
  static int m_currentWinIdInQueue;
  std::thread m_handleQueueThread;
  jimber::CommandQueue m_queue;
  std::mutex mtx;
  bool m_rendererDisabled;

 public:
  HandlerMethod m_handlerMethod;
  newHandlerMethod newm_handlerMethod;

  JCommandHandler(JCommandHandler const&) = delete;
  void operator=(JCommandHandler const&) = delete;
  static void handleQueue();
  static void handleQueueRepeat();
  void sendCommand(int windowId, const jimber::Command& command);

  void setFunction(HandlerMethod h) { m_handlerMethod = h; }
  void setNewFunction(newHandlerMethod h) { newm_handlerMethod = h; }
  void handleCommand(const std::string& command) {
    return;
    // if (m_handlerMethod)
    // m_handlerMethod("dom;" + command);
  }

  void handleNewCommand(const char* ptr, int size) {
    if (newm_handlerMethod)
      newm_handlerMethod(ptr, size);
  }

  void sendDom(Document* document);
  void sendImage(const Document* document,
                 const std::string& mimeType,
                 const std::string& url,
                 Image* image,
                 bool dataUrl,
                 const std::string& idMatch);
  void sendFont(Document* document,
                const std::string& mimeType,
                const std::string& url,
                SharedBuffer* font,
                bool dataUrl,
                WTF::String imageUuid);
  void sendAppendChild(Node* parentNode, Node* newChild);
  void sendInsertBefore(Node* parent, Node* refChild, Node* newChild);
  void sendReplaceChild(Node*, Node*);
  void sendRemoveChild(Node* toRemove);
  void sendCssPropertyUpdate(Element* element,
                             const std::string& propertyName,
                             const std::string& propertyValue);
  void sendCssTextUpdate(Element* element, std::string& cssText);
  void sendSetAttribute(Node* node,
                        const std::string& attributeName,
                        const std::string& attributeValue,
                        bool null);
  void sendSetCharacterData(Node* node, const std::string& value);
  void sendInsertCssRule(Node* node,
                         const std::string& rule,
                         int ruleIndex,
                         const std::string& uuid);
  void sendDeleteCssRule(Node* node, int ruleIndex, const std::string& uuid);
  void sendSetScroll(Node* node);
  void sendDispatchEvent(Node* node, const std::string& type);
  void sendSetFocus(Node* node, bool flag);
  void sendInsertTextIntoNode(Node* node, int offset, const std::string& text);
  void sendDeleteTextFromNode(Node* node, int offset, int count);
  void sendReplaceTextInNode(Node* node,
                             int offset,
                             int count,
                             const std::string& text);
  void sendSetInputSelection(Node* node, unsigned start, unsigned end);
  void sendSetValue(Node* node, const std::string& value);
  void sendSetSelection(Document* document,
                        Node* startNode,
                        int startOffset,
                        int startType,
                        Node* endNode,
                        int endOffset,
                        int endType);
  void sendSetHovered(Node* node, bool hovered);
  void sendAttachShadow(Node* node, ShadowRoot* shadowRoot);
  void sendCreateFontFace(Document* document,
                          const std::string& fontName,
                          const std::string& fontFile,
                          int fontStyle,
                          int fontWeight,
                          int fontStretch);
  void sendCreateFontFace(ExecutionContext* context,
                          FontFace* fontFace,
                          std::string source);
  void sendSetImageForElement(Element* element, const std::string& uuid);
  // media stuff
  void sendAppendDataToSourceBuffer(int winId,
                                    const std::string& mediaSourceId,
                                    const std::string& id,
                                    const double timestampOffset,
                                    const unsigned char* data,
                                    unsigned length);
  void sendMediaElementAction(Element* element,
                              jimber::MediaElementAction action,
                              double value);
  void sendAddSourceBuffer(int winId,
                           const std::string& id,
                           const std::string& UUID,
                           const std::string& type,
                           const std::string& codecs);
  void sendResetSourceBufferParser(int winId,
                                   const std::string& mediaSourceId,
                                   const std::string& id);
  void sendMediaSourceAction(int winId,
                             const std::string& id,
                             jimber::MediaSourceAction action);
  void sendAttachMediaSourceToElement(Element* element, const std::string& id);
  // void sendSetAppendWindowStartCommand(const std::string& id,
  //                                      const double time);
  // void sendSetAppendWindowEndCommand(const std::string& id, const double
  // time);
  void sendSetHtmlImageForElement(Element* element, const std::string& uuid);
  void sendSetCssImage(const Document* document, const std::string& uuid);
  void sendReplaceCustomFont(const Document* document,
                             const std::string& fontName);

  void sendCreateNode(Node* node);
  void sendDeleteNode(Node* node);
  void sendCreateConstructedStyleSheet(const Document* document,
                                       const std::string& constructedUUID);
  void sendSetAdoptedStyleSheets(
      const Node* node,
      const HeapVector<Member<CSSStyleSheet>>& adopted_style_sheets);
  void sendCSSStyleSheetReplace(const Document* document,
                                const std::string& constructedUUID,
                                const std::string& replaceString,
                                bool sync);
  void sendClearDocument(const Document* document);
  void sendSetChecked(const Node* node, bool checked);
  void sendOpenSelectionMenu(const Node* node,
                             int x,
                             int y,
                             int width,
                             int height,
                             const std::vector<std::string>& options);
  void sendCloseSelectionMenu(const Node* node);
  void sendSelectOption(Node* node, int index);
  // void sendCanvasVideoData(int winId, int nodeId, const std::string&
  // encoded_data);
  void sendCanvasVideoData(const Node* node,
                           int realWidth,
                           int realHeight,
                           const char*,
                           unsigned length);
  void sendCanvasImageData(const Node* node,
                           int realWidth,
                           int realHeight,
                           const char*,
                           unsigned length);
  void sendClearCanvas(const Node* node);

  void sendImageWithBlobUrl(const Element* element,
                            Image* image,
                            const std::string blobUrl,
                            const std::string mimeType);

  void sendFullScreenRequest(const Node* node);

  void sendAttachFontFaceToDocument(const Document* document,
                                    std::string fontUuid);
};

}  // namespace blink
