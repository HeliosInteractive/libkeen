#pragma once

#include "keen.hpp"

namespace libkeen
{

/*
 * @class Client
 * @brief The only publicly accessible Class of this
 *        library. As soon as one instance of Client
 *        is constructed, Core library is started and
 *        internal thread pool starts listening.
 * @note  Client API is thread-safe
 */
class Client
{
public:
    explicit            Client();
    virtual             ~Client();
    //! Send an event to a Metric endpoint.
    void                sendEvent(const std::string& collection, const std::string& json);
    //! Sets project ID associated with this client
    void                setProjectId(const std::string& id);
    //! Sets Write Key associated with this client
    void                setWriteKey(const std::string& key);
    //! Gets project ID associated with this client
    std::string         getProjectId() const;
    //! Gets Write Key associated with this client
    std::string         getWriteKey() const;

protected:
    //! Client sub-classes have a chance to provide their own end-point
    virtual std::string getEndpoint(const std::string& collection) const;

private:
    mutable std::mutex  mClientLock;
    CoreRef             mCoreRef;
    std::string         mProjectId;
    std::string         mWriteKey;
};

}
