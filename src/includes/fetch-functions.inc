#!/bin/bash

# Function to get subscriptions
function get_subscriptions() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESULTS=""

    if [[ $l_SUBSCRIPTION_ID == "" ]]; then
        # Get all subscriptions if no specific subscription ID provided
        l_RESULTS=$(az account subscription list --output="json" 2>/dev/null)
    else
        # Get specific subscription by ID
        l_RESULTS=$(az account subscription list --query "[?subscriptionId=='$l_SUBSCRIPTION_ID']" --output="json" 2>/dev/null)
    fi

    echo "$l_RESULTS"
}

# Function to get resource groups
function get_resource_groups() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    local l_RESULTS=""

    if [[ $l_RESOURCE_GROUP_NAME == "" ]]; then
        # Get all resource groups if no specific resource group name provided
        l_RESULTS=$(az group list --output="json" --subscription="$l_SUBSCRIPTION_ID" 2>/dev/null)
    else
        # Get specific resource group by name
        l_RESULTS=$(az group list --output="json" --subscription="$l_SUBSCRIPTION_ID" --query "[?name=='$l_RESOURCE_GROUP_NAME']" 2>/dev/null)
    fi

    echo "$l_RESULTS"
}

# Function to get Network Security Groups
function get_network_security_groups() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    
    # Get Security Rules
    az network nsg list \
        --output="json" \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" \
        2>/dev/null  \
        | jq -r 'map({ name: .name, location: .location, securityRules: (.securityRules | map(del(.etag))) })'
}

# Function to get members of a group
function get_group_members() {
    local l_GROUP_NAME="$1"
    
    # Get group members
    az ad group member list --group "$GROUP_NAME" --output="json"
}

# Function to get members of a group and serialize the emails
function get_group_members_serialized() {
    local l_GROUP_NAME="$1"
    local l_EMAILS=""
    local l_PRINCIPLE_EMAIL=""

    # Get group members and extract email addresses
    l_GROUP_MEMBERS=$(az ad group member list --group "$l_GROUP_NAME" --query "[].mail" --output="json" 2>/dev/null)
    
    # Iterate over each member's email address
    for l_MEMBER in $(echo "$l_GROUP_MEMBERS" | jq -r '.[]'); do
        if [[ $l_MEMBER != "" ]]; then
            l_MEMBERS="$l_MEMBERS$l_MEMBER;"
        fi
    done

    echo "$l_MEMBERS"
}

# Function to get role assignments for a subscription
function get_subscription_role_assignments() {
    local l_SUBSCRIPTION_NAME="$1"
    
    # Get subscription role assignments
    az role assignment list --subscription "$l_SUBSCRIPTION_NAME" --output="json" 2>/dev/null
}

# Function to get role assignments for a resource group
function get_resource_group_role_assignments() {
    local l_SUBSCRIPTION_NAME="$1"
    local l_RESOURCE_GROUP_NAME="$2"

    # Get resource group role assignments
    az role assignment list \
        --subscription "$l_SUBSCRIPTION_NAME" \
        --resource-group "$l_RESOURCE_GROUP_NAME" \
        --output="json"
}

function get_storage_accounts() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    
    # Get Storage Accounts
    az storage account list \
        --output="json" \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" 2> /dev/null
}

function get_storage_account_containers() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    local l_STORAGE_ACCOUNT_NAME="$3"

    # Get Storage Account Containers
    az storage container list \
        --subscription "$l_SUBSCRIPTION_ID" \
        --resource-group "$l_RESOURCE_GROUP_NAME" \
        --account-name "$l_STORAGE_ACCOUNT_NAME" \
        --output="json" 2>/dev/null
}

# Function to get Azure SQL servers
function get_azure_sql_servers() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    
    # Get Azure SQL servers
    az sql server list \
        --output="json" \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" 2>/dev/null
}

# Function to get Azure SQL server firewall rules
function get_azure_sql_server_firewall_rules() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    local l_SQL_SERVER_NAME="$3"
    
    # Get Azure SQL server firewall rules
    az sql server firewall-rule list \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" \
        --server="$l_SQL_SERVER_NAME" \
        --output="json" 2>/dev/null
}

# Function to get SQL Server Audit Policy
function get_sql_server_audit_policy() {
    local l_sql_server_name="$1"
    local l_resource_group_name="$2"
    local l_subscription_id="$3"

    # Get SQL Server Audit Policy
    az sql server audit-policy show \
        --name "$l_sql_server_name" \
        --resource-group "$l_resource_group_name" \
        --subscription "$l_subscription_id" \
        --output="json" 2>/dev/null
}

function get_postgres_servers() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    
    # Get Postgres Servers
    az postgres server list \
        --output="json" \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" 2>/dev/null
}

function get_postgres_server_firewall_rules() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    local l_POSTGRES_SERVER_NAME="$3"
    
    # Get Postgres Server Firewall Rules
    az postgres server firewall-rule list \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" \
        --server-name="$l_POSTGRES_SERVER_NAME" \
        --output="json" 2>/dev/null
}

function get_postgres_flexible_servers() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    
    # Get Postgres Flexible Servers
    az postgres flexible-server list \
        --output="json" \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" 2>/dev/null
}

function get_postgres_flexible_server_firewall_rules() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    local l_POSTGRES_FLEXIBLE_SERVER_NAME="$3"
    
    # Get Postgres Flexible Server Firewall Rules
    az postgres flexible-server firewall-rule list \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" \
        --name="$l_POSTGRES_FLEXIBLE_SERVER_NAME" \
        --output="json" 2>/dev/null
}

function get_mysql_servers() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    
    # Get MySQL Servers
    az mysql server list \
        --output="json" \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" 2>/dev/null
}

function get_mysql_server_firewall_rules() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    local l_MYSQL_SERVER_NAME="$3"
    
    # Get MySQL Server Firewall Rules
    az mysql server firewall-rule list \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" \
        --name="$l_MYSQL_SERVER_NAME" \
        --output="json" 2>/dev/null
}

function get_mysql_flexible_servers() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    
    # Get MySQL Flexible Servers
    az mysql flexible-server list \
        --output="json" \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" 2>/dev/null
}

function get_mysql_flexible_server_firewall_rules() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    local l_MYSQL_FLEXIBLE_SERVER_NAME="$3"
    
    # Get MySQL Flexible Server Firewall Rules
    az mysql flexible-server firewall-rule list \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" \
        --server-name="$l_MYSQL_FLEXIBLE_SERVER_NAME" \
        --output="json" 2>/dev/null
}

function get_mariadb_servers() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    
    # Get MariaDB Servers
    az mariadb server list \
        --output="json" \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" 2>/dev/null
}

function get_mariadb_server_firewall_rules() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"
    local l_MARIADB_SERVER_NAME="$3"
    
    # Get MariaDB Server Firewall Rules
    az mariadb server firewall-rule list \
        --subscription="$l_SUBSCRIPTION_ID" \
        --resource-group="$l_RESOURCE_GROUP_NAME" \
        --server-name="$l_MARIADB_SERVER_NAME" \
        --output="json" 2>/dev/null
}

function get_key_vaults() {
    local l_SUBSCRIPTION_ID=$1
    local l_RESOURCE_GROUP_NAME=$2

    # Get Key Vaults list
    az keyvault list \
        --subscription "$l_SUBSCRIPTION_ID" \
        --resource-group "$l_RESOURCE_GROUP_NAME" \
        --output="json" 2>/dev/null
}

function get_specific_key_vault_information() {
    local l_SUBSCRIPTION_ID=$1
    local l_RESOURCE_GROUP_NAME=$2
    local l_KEY_VAULT_NAME=$3

    # Get specific Key Vault information
    az keyvault show \
        --subscription "$l_SUBSCRIPTION_ID" \
        --resource-group "$l_RESOURCE_GROUP_NAME" \
        --name "$l_KEY_VAULT_NAME" \
        --output="json" 2>/dev/null
}

function get_storage_account_private_endpoint_access() {
    local l_STORAGE_ACCOUNT_NAME=$1
    local l_RESOURCE_GROUP_NAME=$2
    local l_SUBSCRIPTION_ID=$3
    
    # Get Storage Account Private Endpoint Connections
    az storage account show \
        --subscription "$l_SUBSCRIPTION_ID" \
        --resource-group "$l_RESOURCE_GROUP_NAME" \
        --name "$l_STORAGE_ACCOUNT_NAME" \
        --query "privateEndpointConnections" \
        --output="json" 2>/dev/null
}

function get_storage_account_network_default() {
    local l_STORAGE_ACCOUNT_NAME=$1
    local l_RESOURCE_GROUP_NAME=$2
    local l_SUBSCRIPTION_ID=$3
    
    # Get Storage Account Network Default Action - might need to just do: networkRuleSet.defaultAction <--or try networkRuleSet.DefaultAction
    az storage account show \
        --subscription "$l_SUBSCRIPTION_ID" \
        --resource-group "$l_RESOURCE_GROUP_NAME" \
        --name "$l_STORAGE_ACCOUNT_NAME" \
        --query "networkRuleSet" \
        --output="json" 2>/dev/null
}

function get_storage_account_allow_blob_public_access() {
    local l_STORAGE_ACCOUNT_NAME=$1
    local l_RESOURCE_GROUP_NAME=$2
    local l_SUBSCRIPTION_ID=$3
    
    # Get Storage Account Allow Blob Public Access - might just need name and not subscription nor resource name
    az storage account show \
        --subscription "$l_SUBSCRIPTION_ID" \
        --resource-group "$l_RESOURCE_GROUP_NAME" \
        --name "$l_STORAGE_ACCOUNT_NAME" \
        --query "allowBlobPublicAccess" \
        --output="json" 2> /dev/null
}

function get_azure_redis_list() {
    local l_SUBSCRIPTION_ID=$1
    local l_RESOURCE_GROUP_NAME=$2

    az redis list \
        --subscription "$l_SUBSCRIPTION_ID" \
        --resource-group "$l_RESOURCE_GROUP_NAME" \
        --output="json" 2>/dev/null
}

function get_azure_redis_details() {
    local l_SUBSCRIPTION_ID=$1
    local l_RESOURCE_GROUP_NAME=$2
    local l_RADIS_CACHE_NAME=$3

    az redis show \
        --name "$l_RADIS_CACHE_NAME" \
        --resource-group "$l_RESOURCE_GROUP_NAME" \
        --subscription "$l_SUBSCRIPTION_ID" \
        --output="json" 2>/dev/null
}

function get_cosmosdbs() {
    local l_SUBSCRIPTION_ID="$1"
    local l_RESOURCE_GROUP_NAME="$2"

    az cosmosdb list \
        --subscription "$l_SUBSCRIPTION_ID" \
        --resource-group "$l_RESOURCE_GROUP_NAME" \
        --output="json" 2>/dev/null
}

function get_cosmosdb_details() {
    local l_SUBSCRIPTION_ID=$1
    local l_RESOURCE_GROUP_NAME=$2
    local l_COSMOSDB_NAME=$3

    #--name might have to be --server-name or --account-name
    az cosmosdb show \
        --subscription "$l_SUBSCRIPTION_ID" \
        --resource-group "$l_RESOURCE_GROUP_NAME" \
        --name "$l_COSMOSDB_NAME" \
        --output="json" 2>/dev/null
}

function get_role_assignments() {
    local l_SUBSCRIPTION_ID=$1
    local l_RESOURCE_GROUP_NAME=$2

    az role assignment list \
        --subscription "$l_SUBSCRIPTION_ID" \
        --resource-group "$l_RESOURCE_GROUP_NAME" \
        --all \
        --output="json" 2>/dev/null
}

function get_mfa_enforced() {
    local l_PRINCIPAL_NAME=$1

    az ad user show \
        --id "$l_PRINCIPAL_NAME" \
        --query "userState"
        --output="json" 2>/dev/null
}

function get_mfa_exempt_policy() {
    local l_PRINCIPAL_NAME=$1

    az ad conditional-access policy list \
        --query "[?contains(excludeUsers, '$IDENTITY')]" \
        --output="json" 2>/dev/null
}