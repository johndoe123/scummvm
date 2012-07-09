/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/*
 * This file is based on WME Lite.
 * http://dead-code.org/redir.php?target=wmelite
 * Copyright (c) 2011 Jan Nedoma
 */

#include "engines/wintermute/Base/BGame.h"
#include "engines/wintermute/Base/BRegistry.h"
#include "engines/wintermute/Base/scriptables/SXStore.h"
#include "engines/wintermute/Base/scriptables/ScValue.h"
#include "engines/wintermute/Base/scriptables/ScScript.h"
#include "engines/wintermute/Base/scriptables/ScStack.h"
#include "engines/wintermute/utils/StringUtil.h"

#ifdef __IPHONEOS__
#   include "IOS_StoreKit_interface.h"
#endif

namespace WinterMute {

IMPLEMENT_PERSISTENT(CSXStore, false)

CBScriptable *makeSXStore(CBGame *inGame, CScStack *stack) {
	return new CSXStore(inGame);
}

//////////////////////////////////////////////////////////////////////////
CSXStore::CSXStore(CBGame *inGame) : CBObject(inGame) {
#ifdef __IPHONEOS__
	StoreKit_SetExternalData((void *)this);
#endif

	_eventsEnabled = false;
	_lastProductRequestOwner = NULL;
	_lastPurchaseOwner = NULL;
	_lastRestoreOwner = NULL;
}


//////////////////////////////////////////////////////////////////////////
CSXStore::~CSXStore() {
	cleanup();
}

//////////////////////////////////////////////////////////////////////////
void CSXStore::cleanup() {
	setEventsEnabled(NULL, false);

	for (int i = 0; i < _validProducts.GetSize(); i++) {
		delete _validProducts[i];
	}
	_validProducts.RemoveAll();


	for (int i = 0; i < _transactions.GetSize(); i++) {
		delete _transactions[i];
	}
	_transactions.RemoveAll();


	_lastProductRequestOwner = _lastPurchaseOwner = _lastRestoreOwner = NULL;
}

//////////////////////////////////////////////////////////////////////////
ERRORCODE CSXStore::scCallMethod(CScScript *script, CScStack *stack, CScStack *thisStack, const char *name) {
	//////////////////////////////////////////////////////////////////////////
	// EnableEvents
	//////////////////////////////////////////////////////////////////////////
	if (strcmp(name, "EnableEvents") == 0) {
		stack->correctParams(0);
		setEventsEnabled(script, true);
		stack->pushBool(getEventsEnabled() == true);
		return STATUS_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// DisableEvents
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "DisableEvents") == 0) {
		stack->correctParams(0);
		setEventsEnabled(script, false);
		stack->pushBool(getEventsEnabled() == false);
		return STATUS_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// ValidateProducts
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "ValidateProducts") == 0) {
		stack->correctParams(1);
		const char *prodIdList = stack->pop()->getString();
		_lastProductRequestOwner = script->_owner;
		validateProducts(prodIdList);
		stack->pushNULL();
		return STATUS_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// GetValidProduct
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "GetValidProduct") == 0) {
		stack->correctParams(1);
		int index = stack->pop()->getInt();
		if (index >= 0 && index < _validProducts.GetSize()) {
			CScValue *prod = stack->getPushValue();
			if (prod) {
				prod->setProperty("Id", _validProducts[index]->getId());
				prod->setProperty("Name", _validProducts[index]->getName());
				prod->setProperty("Description", _validProducts[index]->getDesc());
				prod->setProperty("Price", _validProducts[index]->getPrice());
			}
		} else
			stack->pushNULL();

		return STATUS_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// GetInvalidProduct
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "GetInvalidProduct") == 0) {
		stack->correctParams(1);
		int index = stack->pop()->getInt();
		if (index >= 0 && (uint32)index < _invalidProducts.size())
			stack->pushString(_invalidProducts[(uint32)index].c_str());
		else
			stack->pushNULL();

		return STATUS_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// GetTransaction
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "GetTransaction") == 0) {
		stack->correctParams(1);
		int index = stack->pop()->getInt();
		if (index >= 0 && index < _transactions.GetSize()) {
			CScValue *trans = stack->getPushValue();
			if (trans) {
				trans->setProperty("Id", _transactions[index]->getId());
				trans->setProperty("ProductId", _transactions[index]->getProductId());
				trans->setProperty("State", _transactions[index]->getState());
			}
		} else
			stack->pushNULL();

		return STATUS_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// Purchase
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "Purchase") == 0) {
		stack->correctParams(1);
		const char *prodId = stack->pop()->getString();
		stack->pushBool(purchase(script, prodId));

		return STATUS_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// FinishTransaction
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "FinishTransaction") == 0) {
		stack->correctParams(1);
		const char *transId = stack->pop()->getString();
		stack->pushBool(finishTransaction(script, transId));

		return STATUS_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// RestoreTransactions
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "RestoreTransactions") == 0) {
		stack->correctParams(0);
		restoreTransactions(script);
		stack->pushNULL();

		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// UnlockProduct
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "UnlockProduct") == 0) {
		stack->correctParams(1);
		const char *prodId = stack->pop()->getString();

		Game->_registry->writeBool("Purchases", prodId, true);
		Game->_registry->saveValues();

		stack->pushBool(true);

		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// IsProductUnlocked
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "IsProductUnlocked") == 0) {
		stack->correctParams(1);
		const char *prodId = stack->pop()->getString();

		stack->pushBool(Game->_registry->readBool("Purchases", prodId, false));

		return STATUS_OK;
	}

	else return STATUS_FAILED;
}


//////////////////////////////////////////////////////////////////////////
CScValue *CSXStore::scGetProperty(const char *name) {
	_scValue->setNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////////////////////////////
	if (strcmp(name, "Type") == 0) {
		_scValue->setString("store");
		return _scValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// Available (RO)
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "Available") == 0) {
		_scValue->setBool(isAvailable());
		return _scValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// EventsEnabled (RO)
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "EventsEnabled") == 0) {
		_scValue->setBool(getEventsEnabled());
		return _scValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// NumValidProducts (RO)
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "NumValidProducts") == 0) {
		_scValue->setInt(_validProducts.GetSize());
		return _scValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// NumInvalidProducts (RO)
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "NumInvalidProducts") == 0) {
		_scValue->setInt((int)_invalidProducts.size());
		return _scValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// NumTransactions (RO)
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "NumTransactions") == 0) {
		_scValue->setInt(_transactions.GetSize());
		return _scValue;
	}

	else return _scValue;
}


//////////////////////////////////////////////////////////////////////////
ERRORCODE CSXStore::persist(CBPersistMgr *persistMgr) {
	if (!persistMgr->_saving)
		cleanup();

	CBObject::persist(persistMgr);

	persistMgr->transfer(TMEMBER(_eventsEnabled));
	persistMgr->transfer(TMEMBER(_lastProductRequestOwner));
	persistMgr->transfer(TMEMBER(_lastPurchaseOwner));
	persistMgr->transfer(TMEMBER(_lastRestoreOwner));
	persistMgr->transfer(TMEMBER(_invalidProducts));

	// persist valid products
	int numProducts;
	if (persistMgr->_saving) {
		numProducts = _validProducts.GetSize();
		persistMgr->transfer(TMEMBER(numProducts));
		for (int i = 0; i < numProducts; i++) _validProducts[i]->persist(persistMgr);
	} else {
		numProducts = _validProducts.GetSize();
		persistMgr->transfer(TMEMBER(numProducts));
		for (int i = 0; i < numProducts; i++) {
			CBStoreProduct *prod = new CBStoreProduct;
			prod->persist(persistMgr);
			_validProducts.Add(prod);
		}
	}

	return STATUS_OK;
}


//////////////////////////////////////////////////////////////////////////
void CSXStore::afterLoad() {
	if (_eventsEnabled) {
		setEventsEnabled(NULL, true);
	}
#ifdef __IPHONEOS__
	StoreKit_SetExternalData((void *)this);
#endif
}

//////////////////////////////////////////////////////////////////////////
void CSXStore::OnObjectDestroyed(CBScriptHolder *obj) {
	if (_lastProductRequestOwner == obj) _lastProductRequestOwner = NULL;
	if (_lastPurchaseOwner == obj) _lastPurchaseOwner = NULL;
	if (_lastRestoreOwner == obj) _lastRestoreOwner = NULL;
}

//////////////////////////////////////////////////////////////////////////
void CSXStore::setEventsEnabled(CScScript *script, bool val) {
	_eventsEnabled = val;

	if (val) {
		if (script) _lastPurchaseOwner = script->_owner;
#ifdef __IPHONEOS__
		StoreKit_EnableEvents();
#endif
	} else {
		_lastPurchaseOwner = NULL;
#ifdef __IPHONEOS__
		StoreKit_DisableEvents();
#endif
	}
}

//////////////////////////////////////////////////////////////////////////
void CSXStore::validateProducts(const char *prodIdList) {
#ifdef __IPHONEOS__
	StoreKit_ValidateProducts(prodIdList);
#endif
}

//////////////////////////////////////////////////////////////////////////
bool CSXStore::isAvailable() {
#ifdef __IPHONEOS__
	return StoreKit_IsStoreAvailable() > 0;
#else
	return false;
#endif
}

//////////////////////////////////////////////////////////////////////////
void CSXStore::receiveProductsStart() {
	for (int i = 0; i < _validProducts.GetSize(); i++) {
		delete _validProducts[i];
	}
	_validProducts.RemoveAll();

	_invalidProducts.clear();
}

//////////////////////////////////////////////////////////////////////////
void CSXStore::receiveProductsEnd() {
	if (_lastProductRequestOwner) _lastProductRequestOwner->applyEvent("ProductsValidated");
}

//////////////////////////////////////////////////////////////////////////
void CSXStore::addValidProduct(const char *id, const char *name, const char *desc, const char *price) {
	CBStoreProduct *prod = new CBStoreProduct(id, name, desc, price);
	_validProducts.Add(prod);
}

//////////////////////////////////////////////////////////////////////////
void CSXStore::addInvalidProduct(const char *id) {
	_invalidProducts.push_back(id);
}

//////////////////////////////////////////////////////////////////////////
void CSXStore::receiveTransactionsStart() {
	for (int i = 0; i < _transactions.GetSize(); i++) {
		delete _transactions[i];
	}
	_transactions.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
void CSXStore::receiveTransactionsEnd() {
	if (_lastPurchaseOwner)
		_lastPurchaseOwner->applyEvent("TransactionsUpdated");
	else
		Game->applyEvent("TransactionsUpdated");
}

//////////////////////////////////////////////////////////////////////////
void CSXStore::addTransaction(const char *id, const char *productId, const char *state) {
	CBStoreTransaction *trans = new CBStoreTransaction(id, productId, state);
	_transactions.Add(trans);
}

//////////////////////////////////////////////////////////////////////////
bool CSXStore::purchase(CScScript *script, const char *productId) {
	if (!productId) return false;

#ifdef __IPHONEOS__
	for (int i = 0; i < _validProducts.GetSize(); i++) {
		if (strcmp(productId, _validProducts[i]->GetId()) == 0) {
			_lastPurchaseOwner = script->_owner;

			StoreKit_Purchase(productId);
			return true;
		}
	}
#endif
	script->runtimeError("Store.Purchase() - '%s' is not a valid product id", productId);
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CSXStore::finishTransaction(CScScript *script, const char *transId) {
	if (!transId) return false;
#ifdef __IPHONEOS__
	for (int i = 0; i < _transactions.GetSize(); i++) {
		if (strcmp(transId, _transactions[i]->GetId()) == 0) {
			if (StoreKit_FinishTransaction(transId) > 0) {
				SAFE_DELETE(_transactions[i]);
				_transactions.RemoveAt(i);
				return true;
			} else return false;
		}
	}
#endif
	script->runtimeError("Store.FinishTransaction() - '%s' is not a valid transaction id", transId);
	return false;
}

//////////////////////////////////////////////////////////////////////////
void CSXStore::restoreTransactions(CScScript *script) {
	_lastRestoreOwner = script->_owner;
#ifdef __IPHONEOS__
	StoreKit_RestoreTransactions();
#endif
}

//////////////////////////////////////////////////////////////////////////
void CSXStore::onRestoreFinished(bool error) {
	if (_lastRestoreOwner) {
		if (error) _lastRestoreOwner->applyEvent("TransactionsRestoreFailed");
		else _lastRestoreOwner->applyEvent("TransactionsRestoreFinished");
	}
}



#ifdef __IPHONEOS__

//////////////////////////////////////////////////////////////////////////
// StoreKit callbacks (called from ObjC)
//////////////////////////////////////////////////////////////////////////
void StoreKit_AddValidProductCallback(const char *id, const char *name, const char *desc, const char *price, void *data) {
	CSXStore *store = static_cast<CSXStore *>(data);
	if (store) store->AddValidProduct(id, name, desc, price);
}

//////////////////////////////////////////////////////////////////////////
void StoreKit_AddInvalidProductCallback(const char *id, void *data) {
	CSXStore *store = static_cast<CSXStore *>(data);
	if (store) store->AddInvalidProduct(id);
}

//////////////////////////////////////////////////////////////////////////
void StoreKit_ReceiveProductsStartCallback(void *data) {
	CSXStore *store = static_cast<CSXStore *>(data);
	if (store) store->ReceiveProductsStart();
}

//////////////////////////////////////////////////////////////////////////
void StoreKit_ReceiveProductsEndCallback(void *data) {
	CSXStore *store = static_cast<CSXStore *>(data);
	if (store) store->ReceiveProductsEnd();
}

//////////////////////////////////////////////////////////////////////////
void StoreKit_AddTransactionCallback(const char *id, const char *productId, const char *state, void *data) {
	CSXStore *store = static_cast<CSXStore *>(data);
	if (store) store->AddTransaction(id, productId, state);
}

//////////////////////////////////////////////////////////////////////////
void StoreKit_ReceiveTransactionsStartCallback(void *data) {
	CSXStore *store = static_cast<CSXStore *>(data);
	if (store) store->ReceiveTransactionsStart();
}

//////////////////////////////////////////////////////////////////////////
void StoreKit_ReceiveTransactionsEndCallback(void *data) {
	CSXStore *store = static_cast<CSXStore *>(data);
	if (store) store->ReceiveTransactionsEnd();
}
//////////////////////////////////////////////////////////////////////////
void StoreKit_RestoreFinishedCallback(void *data, int error) {
	CSXStore *store = static_cast<CSXStore *>(data);
	if (store) store->OnRestoreFinished(error > 0);
}

#endif // __IPHONEOS__

} // end of namespace WinterMute
