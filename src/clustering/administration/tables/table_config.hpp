// Copyright 2010-2014 RethinkDB, all rights reserved.
#ifndef CLUSTERING_ADMINISTRATION_TABLES_TABLE_CONFIG_HPP_
#define CLUSTERING_ADMINISTRATION_TABLES_TABLE_CONFIG_HPP_

#include <string>
#include <vector>

#include "errors.hpp"
#include <boost/shared_ptr.hpp>

#include "clustering/administration/database_metadata.hpp"
#include "clustering/administration/namespace_metadata.hpp"
#include "clustering/administration/servers/name_client.hpp"
#include "clustering/administration/tables/table_common.hpp"
#include "containers/uuid.hpp"
#include "rdb_protocol/artificial_table/backend.hpp"
#include "rpc/semilattice/view.hpp"

class real_reql_cluster_interface_t;

/* This is publicly exposed so that it can be used to create the return value of
`table.reconfigure()`. */
ql::datum_t convert_table_config_to_datum(
        const table_config_t &config);

class table_config_artificial_table_backend_t :
    public common_table_artificial_table_backend_t
{
public:
    table_config_artificial_table_backend_t(
            boost::shared_ptr< semilattice_readwrite_view_t<
                cow_ptr_t<namespaces_semilattice_metadata_t> > > _table_sl_view,
            boost::shared_ptr< semilattice_readwrite_view_t<
                databases_semilattice_metadata_t> > _database_sl_view,
            real_reql_cluster_interface_t *_reql_cluster_interface,
            server_name_client_t *_name_client) :
        common_table_artificial_table_backend_t(_table_sl_view, _database_sl_view),
        reql_cluster_interface(_reql_cluster_interface),
        name_client(_name_client) { }

    bool read_row_impl(
            namespace_id_t table_id,
            name_string_t table_name,
            name_string_t db_name,
            const namespace_semilattice_metadata_t &metadata,
            signal_t *interruptor,
            ql::datum_t *row_out,
            std::string *error_out);
    bool write_row(
            ql::datum_t primary_key,
            ql::datum_t new_value,
            signal_t *interruptor,
            std::string *error_out);

private:
    real_reql_cluster_interface_t *reql_cluster_interface;
    server_name_client_t *name_client;
};

#endif /* CLUSTERING_ADMINISTRATION_TABLES_TABLE_CONFIG_HPP_ */

