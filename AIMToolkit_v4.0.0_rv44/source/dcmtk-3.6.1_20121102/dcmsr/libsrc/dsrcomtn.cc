/*
 *
 *  Copyright (C) 2000-2012, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *
 *  Module:  dcmsr
 *
 *  Author:  Joerg Riesmeier
 *
 *  Purpose:
 *    classes: DSRCompositeTreeNode
 *
 */


#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

#include "dcmtk/dcmsr/dsrtypes.h"
#include "dcmtk/dcmsr/dsrcomtn.h"
#include "dcmtk/dcmsr/dsrxmld.h"

#include "dcmtk/dcmdata/dcuid.h"


DSRCompositeTreeNode::DSRCompositeTreeNode(const E_RelationshipType relationshipType)
  : DSRDocumentTreeNode(relationshipType, VT_Composite),
    DSRCompositeReferenceValue()
{
}


DSRCompositeTreeNode::~DSRCompositeTreeNode()
{
}


void DSRCompositeTreeNode::clear()
{
    DSRDocumentTreeNode::clear();
    DSRCompositeReferenceValue::clear();
}


OFBool DSRCompositeTreeNode::isValid() const
{
    return DSRDocumentTreeNode::isValid() && DSRCompositeReferenceValue::isValid();
}


OFCondition DSRCompositeTreeNode::print(STD_NAMESPACE ostream &stream,
                                        const size_t flags) const
{
    OFCondition result = DSRDocumentTreeNode::print(stream, flags);
    if (result.good())
    {
        DCMSR_PRINT_ANSI_ESCAPE_CODE(DCMSR_ANSI_ESCAPE_CODE_DELIMITER)
        stream << "=";
        DCMSR_PRINT_ANSI_ESCAPE_CODE(DCMSR_ANSI_ESCAPE_CODE_ITEM_VALUE)
        result = DSRCompositeReferenceValue::print(stream, flags);
    }
    return result;
}


OFCondition DSRCompositeTreeNode::writeXML(STD_NAMESPACE ostream &stream,
                                           const size_t flags) const
{
    OFCondition result = EC_Normal;
    writeXMLItemStart(stream, flags);
    result = DSRDocumentTreeNode::writeXML(stream, flags);
    stream << "<value>" << OFendl;
    DSRCompositeReferenceValue::writeXML(stream, flags);
    stream << "</value>" << OFendl;
    writeXMLItemEnd(stream, flags);
    return result;
}


OFCondition DSRCompositeTreeNode::readContentItem(DcmItem &dataset)
{
    /* read ReferencedSOPSequence */
    return DSRCompositeReferenceValue::readSequence(dataset, "1" /* type */);
}


OFCondition DSRCompositeTreeNode::writeContentItem(DcmItem &dataset) const
{
    /* write ReferencedSOPSequence */
    return DSRCompositeReferenceValue::writeSequence(dataset);
}


OFCondition DSRCompositeTreeNode::readXMLContentItem(const DSRXMLDocument &doc,
                                                     DSRXMLCursor cursor)
{
    /* retrieve value from XML element "value" */
    return DSRCompositeReferenceValue::readXML(doc, doc.getNamedNode(cursor.gotoChild(), "value"));
}


OFCondition DSRCompositeTreeNode::renderHTMLContentItem(STD_NAMESPACE ostream &docStream,
                                                        STD_NAMESPACE ostream &annexStream,
                                                        const size_t /* nestingLevel */,
                                                        size_t &annexNumber,
                                                        const size_t flags) const
{
    /* render ConceptName */
    OFCondition result = renderHTMLConceptName(docStream, flags);
    /* render Reference */
    if (result.good())
    {
        result = DSRCompositeReferenceValue::renderHTML(docStream, annexStream, annexNumber, flags);
        docStream << OFendl;
    }
    return result;
}